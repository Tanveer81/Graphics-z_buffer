#include <bits/stdc++.h>
#include "bitmap_image.hpp"

using namespace std;

string line;

ifstream config("config.txt");

ifstream stage3("stage3.txt");





class point{

public:

    double x,y,z;

    ~point(){}

};




class Plane{

public:

    double a,b,c,d;

    ~Plane(){}

};





class triangle{

public:

    point points[3];

    int color[3];

    int id;

    bool in = false;

    Plane plane;

    ~triangle(){}

};





class edge{

public:

    double delx, ymin, ymax, xatymin, xa;

    int id;

//    int disymin, disymax, disxatymin;

};





void plane_eq(point* a, point* b, point* c, Plane* p){

    point t1,t2;

    t1.x = c->x - a->x;

    t1.y = c->y - a->y;

    t1.z = c->z - a->z;

    t2.x = c->x - b->x;

    t2.y = c->y - b->y;

    t2.z = c->z - b->z;

    p->a = t1.y*t2.z - t1.z*t2.y;

    p->b = -t1.x*t2.z + t1.z*t2.x;

    p->c = t1.x*t2.y - t1.y*t2.x;

    p->d = -p->a*c->x -p->b*c->y - p->c*c->z;

    //cout<<"Plane Equation -> "<<p->a<<"x+ "<<p->b<<"y+ "<<p->c<<"z+ "<<p->d<<endl;
}





void readLine(double a[] , ifstream* myfile){

    if (myfile->is_open()){

      getline (*myfile,line);

      vector <string> tokens;

      stringstream check1(line);

      string intermediate;


      while(getline(check1, intermediate, ' '))tokens.push_back(intermediate);


      for(int i = 0; i < tokens.size(); i++){

         a[i] = atof(tokens[i].c_str());

        }

    }

    else cout << "Unable to open file";
}





point intersection(double y , point a, point b){

    point p;

    double t = (y - a.y)/(b.y - a.y);

    p.x = a.x + t * (b.x - a.x);

    p.y = y;

    p.z = a.z + t * (b.z - a.z);

    return p;
}





void create_edge(point a, point b, edge* e){
    double r;

    e->xatymin = a.y<b.y? a.x : b.x;
    //e->disxatymin = round((e->xatymin - left_x) / dx);

    e->xa = e->xatymin;


    e->ymax = a.y>b.y? a.y : b.y;
//    e->disymax = round((top_y - e->ymax) / dy);


    e->ymin = a.y<b.y? a.y : b.y;
    //e->disymin = round((top_y - e->ymin) / dy);



    double m = (a.y - b.y) / (a.x - b.x);

    double dely = 1;

    e->delx = dely / m;

}

double screen[2]; ///width* height

double x_limit;

double y_limit;

double z_limit[2]; ///front * near

double a[1];

double tr[3];

int screen_width,screen_height;

double xl_limit,yt_limit,zf_limit,xr_limit,yb_limit,zn_limit,dx,dy,top_y,left_x,z_max;

double** z_buffer;

point** frame_buffer;

vector<triangle> triangles;///polygon table

vector<int> apt;///active polygon table

vector<edge> aet; ///active edge table

int number_of_triangles = 0;



bool cmp (edge a, edge b){
    return a.xatymin < b.xatymin;
}



int main()
{
    ///1. read_data() ​:
    readLine(screen, &config);screen_width = screen[0];screen_height = screen[1];

    readLine(a, &config);

    xl_limit = a[0];

    xr_limit = -xl_limit;


    readLine(a, &config);

    yb_limit = a[0];

    yt_limit = -yb_limit;


    readLine(z_limit, &config);

    zn_limit = z_limit[0];

    zf_limit = z_limit[1];


    cout<<"Config File Reading \n";

    cout<< screen_width << " " << screen_height<<"\n";

    cout<< xl_limit << " xl_limit  xr_limit "<< xr_limit << "\n";

    cout<< yt_limit << " yt_limit  yb_limit "<< yb_limit << "\n";

    cout<< zn_limit<< " zn_limit  zf_limit " << zf_limit<<"\n";


    config.close();



    dx = 2*abs(xl_limit)/screen_width;

    dy = 2*abs(yb_limit)/screen_height;

    cout<< dx << "  dx dy  "<< dy<<"\n";


    top_y= yt_limit - dy/2;

    left_x= xl_limit + dx/2;

    cout<<top_y << " top_y left_x " << left_x<<"\n";


    z_max = abs(zn_limit - zf_limit);

    cout<<"z_max "<<z_max<<"\n";


    vector<edge> edge_table[screen_height]; ///edge table

    cout<<"Stage3 File Reading \n";




    while ( getline (stage3,line) ){

        triangle t;

        t.id = number_of_triangles;


        for(int i= 0 ; i< 3 ; i++){

            readLine(tr, &stage3);

            t.points[i].x = tr[0];

            t.points[i].y = tr[1];

            t.points[i].z = tr[2];

            t.color[i] = rand() % 256;

        }


        plane_eq(&t.points[0], &t.points[1], &t.points[2], &t.plane);

        cout<<"Plane Equation -> "<<t.plane.a<<"x+ "<<t.plane.b<<"y+ "<<t.plane.c<<"z+ "<<t.plane.d<<endl;


        ///2.b. Create polygon table:
        triangles.push_back(t);

        number_of_triangles++;

        ///2.a. Creating EDGE TABLE
        edge e1, e2, e3;

        e1.id = t.id;
        e2.id = t.id;
        e3.id = t.id;

        if(t.points[0].y != t.points[1].y){

            create_edge(t.points[0], t.points[1], &e1);

            double ym = t.points[0].y<t.points[1].y? t.points[0].y : t.points[1].y;

            int r = round((top_y - ym) / dy);

            edge_table[r].push_back(e1);
        }


        if(t.points[2].y != t.points[1].y){

            create_edge(t.points[2], t.points[1], &e2);

            double ym = t.points[2].y<t.points[1].y? t.points[2].y : t.points[1].y;

            int r = round((top_y - ym) / dy);

            edge_table[r].push_back(e2);
        }


        if(t.points[0].y != t.points[2].y){

            create_edge(t.points[0], t.points[2], &e3);

            double ym = t.points[0].y<t.points[2].y? t.points[0].y : t.points[2].y;

            int r = round((top_y - ym) / dy);

            edge_table[r].push_back(e3);
        }
    }
    stage3.close();


    for(int i=0; i<screen_height; i++){

//        std::sort(edge_table[i].begin(),
//              edge_table[i].end(),
//              [](const edge& first, const edge& second)
//        {
//        return first.xatymin < second.xatymin;
//        });
          sort(edge_table[i].begin(), edge_table[i].end(),cmp);

    }

    cout<<"Printing edge table"<<endl;


    for(int i=0; i<screen_height; i++){

        if(edge_table[i].size() != 0){

            cout<<i<<"--- ";

            for(int j=0; j<edge_table[i].size(); j++){
                cout<<"xatymin   ymax\n";
                cout<<edge_table[i].at(j).xatymin<<", "<<edge_table[i].at(j).ymax<<"\n";

            }
            cout<<endl;
        }
    }


    cout<<"number_of_triangles : "<<number_of_triangles<<"\n";


    for(int k= 0 ; k< number_of_triangles ; k++){

        cout<<"Triangle Id : "<<triangles[k].id<<endl;

        for(int i= 0 ; i< 3 ; i++){

            cout<<triangles[k].points[i].x<<"  "<<triangles[k].points[i].y<<"  "<<triangles[k].points[i].z <<"\n";

        }
            cout<<"\n";
    }

    cout<<"RGB Color Of Triangles\n";
    for(int k= 0 ; k< number_of_triangles ; k++){

        for(int i= 0 ; i< 3 ; i++){

            cout<<triangles[k].color[i]<<" ";
        }
        cout<<"\n";
    }


    ///create frame buffer

    frame_buffer = new point* [screen_width];

    point c;

    c.x = 0;

    c.y = 0;

    c.z = 0;

    for(int i= 0 ; i< screen_width ; i++){

        frame_buffer[i] = new point[screen_height];

        for(int j = 0 ; j < screen_height ; j++){

              frame_buffer[i][j] = c;

        }

    }





    ///3. apply_procedure():-

    cout<<"Debug   "<<edge_table[0].size();

    for(int y = 0; y<screen_height; y++){

        //cout<<y<<"--- \n";

        double contY = top_y- y*dy;


        if(edge_table[y].size() != 0){

            for(int j=0; j<edge_table[y].size(); j++){

                int polygon_id = edge_table[y].at(j).id;

                ///triangles[polygon_id].in = 1;

                apt.push_back(triangles[polygon_id].id);

                aet.push_back(edge_table[y].at(j));
            }
        }

//        if(aet.size() != 0){
//            for(int j=0; j<aet.size()-1; j++){
//
//                cout << j << j << endl;
//
//                cout<<aet.at(j).xatymin<<endl;
//
//            }
//        }

        //cout << "aet sorting begin\n";

//        std::sort(aet.begin(),
//              aet.end(),
//              [](const edge& first, const edge& second)
//        {
//        return first.xatymin < second.xatymin;
//        });
          sort(aet.begin(), aet.end(),cmp);



        //cout << "aet sorting end\n";


        if(aet.size() != 0){
            for(int j=0; j<aet.size()-1; j++){

                //cout << j << endl;

                //cout<<aet.at(j).xatymin<<endl;

                int polygon_id = aet.at(j).id;

                int t = triangles[polygon_id].in;

                triangles[polygon_id].in = !triangles[polygon_id].in;


                double zmin = INT_MAX;

                int triangle_id = -1;

                for(int i=0; i<triangles.size(); i++){

                    if( triangles[i].in ==  true ){

                        Plane p = triangles[i].plane;

                        double z1 = (-p.d - p.a*aet.at(j).xa -p.b*contY) / p.c;

                        if(zmin > z1){

                            zmin = z1;

                            triangle_id = i;

                        }

                    }

                }

                if(triangle_id == -1)continue;

                cout<<"Y "<< y <<" selected "<<triangle_id<<" Zmin "<<zmin<<endl;

            ///draw
                double xmin = aet.at(j).xa;
                double xmax = aet.at(j+1).xa;
                if( xmin < xl_limit ) xmin = xl_limit;
                if( xmax > xr_limit ) xmax = xr_limit;


                for( double x=xmin; x<xmax; x+=dx){



                    int c = round((x - left_x) / dx);

                    frame_buffer[screen_height - y][c].x = triangles[triangle_id].color[0];
                    frame_buffer[screen_height - y][c].y = triangles[triangle_id].color[1];
                    frame_buffer[screen_height - y][c].z = triangles[triangle_id].color[2];


                }


            }

            ///draw

                cout<<"aet.size()  "<<aet.size()<<endl;

                for(int j=0; j<aet.size(); j++){

                    int r = round( (aet.at(j).ymax - yb_limit) / dy );

                    //r = screen_height - r;

                    //cout<<"r "<<r<<endl;

                    if( r <= y ){



                        cout<< "PAISI"<<endl;

                        ///int polygon_id = aet.at(j).id;

                        aet.erase(aet.begin() + j);

                        //apt.erase(std::remove(apt.begin(), apt.end(), polygon_id), apt.end());

                        j--;

                        ///triangles[polygon_id].in = 0;
                    }
                }

                cout<<"aet.size()  "<<aet.size()<<endl;


            if(aet.size() != 0){
                for(int j=0; j<aet.size(); j++){

                    aet.at(j).xa += (aet.at(j).delx * dy);////////

        //            if( aet.at(j).xa < xl_limit ) aet.at(j).xa = xl_limit;
        //
        //            if( aet.at(j).xa > xr_limit ) aet.at(j).xa = xr_limit;

                }





//                std::sort(aet.begin(),
//                      aet.end(),
//                      [](const edge& first, const edge& second)
//                {
//                return first.xatymin < second.xatymin;
//                });
                  sort(aet.begin(), aet.end(),cmp);

            }


            for(int j=0; j<triangles.size(); j++){

                triangles[j].in = false;

            }


        }


    }






    ///output file

    //ofstream out("z_buffer.txt");

    //out.precision(6);


    ///save image

    bitmap_image image(screen_width,screen_height);

    for(int i= 0 ; i< screen_width ; i++){

        for(int j = 0 ; j < screen_height ; j++){

            //out<<fixed<<i<<", "<<j<<", "<<z_buffer[i][j]<<", "<<frame_buffer[i][j].x<<", "<<frame_buffer[i][j].y<<", "<<frame_buffer[i][j].z<<"\n";

            image.set_pixel(j,i,frame_buffer[i][j].x,frame_buffer[i][j].y,frame_buffer[i][j].z);

        }

    }




    image.save_image("output.bmp");

    //out.close();




    ///free memory

    delete[] frame_buffer;

    vector<triangle>().swap(triangles);




    return 0;
}
