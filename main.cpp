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

class triangle{
public:
    point points[3];
    int color[3];
    ~triangle(){}
};


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

vector<triangle> triangles;
int number_of_triangles = 0;

int main()
{
    ///2. read_data() ​:
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

    cout<<"Stage3 File Reading \n";
    while ( getline (stage3,line) ){
        triangle t;
        for(int i= 0 ; i< 3 ; i++){
            readLine(tr, &stage3);
            t.points[i].x = tr[0];
            t.points[i].y = tr[1];
            t.points[i].z = tr[2];
            t.color[i] = rand() % 256;
        }
        triangles.push_back(t);
        number_of_triangles++;
    }
    stage3.close();

    cout<<"number_of_triangles : "<<number_of_triangles<<"\n";

    for(int k= 0 ; k< number_of_triangles ; k++){
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

    ///3. initialize_z_buffer_and_frame_buffer():
    dx = 2*abs(xl_limit)/screen_width;
    dy = 2*abs(yb_limit)/screen_height;
    cout<< dx << "  dx dy  "<< dy<<"\n";

    top_y= yt_limit - dy/2;
    left_x= xl_limit + dx/2;
    cout<<top_y << " top_y left_x " << left_x<<"\n";

    z_max = abs(zn_limit - zf_limit);
    cout<<"z_max "<<z_max<<"\n";

    z_buffer = new double*[screen_width];
    frame_buffer = new point* [screen_width];
    point c;
    c.x = 0;
    c.y = 0;
    c.z = 0;

    for(int i= 0 ; i< screen_width ; i++){
        z_buffer[i] = new double[screen_height];
        frame_buffer[i] = new point[screen_height];
        for(int j = 0 ; j < screen_height ; j++){
              z_buffer[i][j] = z_max*100000;
              frame_buffer[i][j] = c;
        }
    }


    ///4. ​apply_procedure():

    for (int k = 0 ; k< triangles.size() ; k++){

        cout<<"k "<<k<<endl;

        ///ymax ymin
        double ymax = triangles[k].points[0].y;
        double ymin = triangles[k].points[0].y;
        for (int i = 1; i < 3; i++){
            if(triangles[k].points[i].y > ymax){
                ymax = triangles[k].points[i].y;
            }
            if(triangles[k].points[i].y < ymin){
                ymin = triangles[k].points[i].y;
            }
        }
        cout<<ymax<<" ymax  ymin "<<ymin<<"\n";

        ///xmax xmin
        double xmax = triangles[k].points[0].x;
        double xmin = triangles[k].points[0].x;
        for (int i = 1; i < 3; i++){
            if(triangles[k].points[i].x > xmax){
                xmax = triangles[k].points[i].x;
            }
            if(triangles[k].points[i].x < xmin){
                xmin = triangles[k].points[i].x;
            }
        }
        cout<<xmax<<" xmax  xmin "<<xmin<<"\n";

        double top_scanline = ymax<yt_limit? ymax : yt_limit;
        double bottom_scanline = ymin>yb_limit? ymin : yb_limit;

        int r = round((top_y - top_scanline) / dy);

        double right_column = xmax<xr_limit? xmax : xr_limit;
        double left_column  = xmin>xl_limit? xmin : xl_limit;

        for(double y = top_scanline ; y >= bottom_scanline ; y-=dy){
            point left,right;
            point c1 = intersection(y,triangles[k].points[0],triangles[k].points[1]);
            point c2 = intersection(y,triangles[k].points[1],triangles[k].points[2]);
            point c3 = intersection(y,triangles[k].points[2],triangles[k].points[0]);

            if((c1.x >= xmin && c1.x <= xmax ) && (c2.x >= xmin && c2.x <= xmax)){
                left = c1;
                right = c2;
            }
            else if((c2.x >= xmin && c2.x <= xmax ) && (c3.x >= xmin && c3.x <= xmax)){
                left = c2;
                right = c3;
            }
            else if((c3.x >= xmin && c3.x <= xmax ) && (c1.x >= xmin && c1.x <= xmax)){
                left = c3;
                right = c1;
            }

            if(left.x > right.x){
                point temp = right;
                right = left;
                left = temp;
            }

            double left_intersecting_column  = left.x>left_column? left.x : left_column;
            double right_intersecting_column  = right.x<right_column? right.x : right_column;

            int c = round((left_intersecting_column - left_x) / dx);

            for(double x = left_intersecting_column; x<=right_intersecting_column;x+=dx){
                double t = (x - left.x) / (right.x - left.x);
                double z = left.z + t * (right.z - left.z);
                int comp = z*100000;
                if(comp >= zf_limit && comp < z_buffer[r][c]){
                    z_buffer[r][c] = comp;
                    frame_buffer[r][c].x = triangles[k].color[0];
                    frame_buffer[r][c].y = triangles[k].color[1];
                    frame_buffer[r][c].z = triangles[k].color[2];
                }
                c++;
            }
            r++;
        }

    }

    ///output file

    ofstream out("z_buffer.txt");
    out.precision(6);


    ///save image
    bitmap_image image(screen_width,screen_height);
    for(int i= 0 ; i< screen_width ; i++){
        for(int j = 0 ; j < screen_height ; j++){
            out<<fixed<<i<<", "<<j<<", "<<z_buffer[i][j]<<", "<<frame_buffer[i][j].x<<", "<<frame_buffer[i][j].y<<", "<<frame_buffer[i][j].z<<"\n";
            image.set_pixel(j,i,frame_buffer[i][j].x,frame_buffer[i][j].y,frame_buffer[i][j].z);
        }
    }

    image.save_image("output.bmp");
    out.close();

    ///free memory
    delete[] z_buffer;
    delete[] frame_buffer;
    vector<triangle>().swap(triangles);

    return 0;
}
