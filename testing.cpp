#include <bits/stdc++.h>
#include "bitmap_image.hpp"

using namespace std;

string line;
ifstream config("config.txt");
ifstream stage3("stage3.txt");



class triangle{
    public:
        double points[3][3];
        int color[3];
};

triangle triangles[1000];
int number_of_triangles = 0;


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

int ab;
int main()
{
    double screen[2]; ///width* height
    double x_limit;
    double y_limit;
    double z_limit[2]; ///front * near
    double a[1];
    double tr[3];

    ///2. read_data() ​:
    readLine(screen, &config);
    readLine(a, &config);
    x_limit = a[0];
    readLine(a, &config);
    y_limit = a[0];
    readLine(z_limit, &config);
    cout<<"Config File Reading \n";
    cout<< screen[0] << " " << screen[1] << " "<< x_limit << " "<< y_limit << " "<< z_limit[0]<< " " << z_limit[1]<<"\n\n";

    cout<<"Stage3 File Reading \n";
    while ( getline (stage3,line) ){
        triangle* t = new triangle();
        for(int i= 0 ; i< 3 ; i++){
            readLine(tr, &stage3);
            for(int j = 0 ; j < 3 ; j++){
                t->points[i][j] = tr[j];
                t->color[j] = rand() % 256;
            }
        }
        triangles[number_of_triangles] = *t;
        number_of_triangles++;
    }

    cout<<"number_of_triangles : "<<number_of_triangles<<"\n";

    for(int k= 0 ; k< number_of_triangles ; k++){
        for(int i= 0 ; i< 3 ; i++){
            for(int j = 0 ; j < 3 ; j++){
                cout<<triangles[k].points[i][j]<<"  ";
            }
            cout<<"\n";
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
    double dx = abs(x_limit*2)/screen[0];
    double dy = abs(y_limit*2)/screen[1];
    cout<< dx << "  dx dy  "<< dy<<"\n";
    double top_y= -y_limit-dy/2;
    double left_x= x_limit+dx/2;

    double z_buffer[(int)screen[0]][(int)screen[1]];
    for(int i= 0 ; i< screen[0] ; i++){
        for(int j = 0 ; j < screen[1] ; j++){
              z_buffer[i][j] = z_limit[1];
        }
    }


    bitmap_image image((int)screen[0],(int)screen[1]);
    for(int i=0;i<(int)screen[0];i++){
        for(int j=0;j<(int)screen[1];j++){
            image.set_pixel(i,j,0,0,0);
        }
    }
//
//    image.save_image("test.bmp");



    ///4. ​apply_procedure():



    for (int k = 0 ; k< number_of_triangles ; k++){
        double ymax = triangles[k].points[0][1];
        int ymax_point = 0;
        double ymin = triangles[k].points[0][1];
        int ymin_point = 0;
        double ymax2,ymin2;
        for (int i = 1; i < 3; i++){
            if(triangles[k].points[i][1] > ymax){
                ymax = triangles[k].points[i][1];
                ymax_point = i;
            }
            if(triangles[k].points[i][1] < ymin){
                ymin = triangles[k].points[i][1];
                ymin_point = i;
            }
        }
        cout<<ymax<<"  "<<ymin<<"\n";

        ///clipping
        if(ymax > -y_limit)ymax = -y_limit;///////////
        if(ymin < y_limit)ymin = y_limit;
        cout<<ymax<<"  "<<ymin<<"\n";

        for(double y = ymax ; y >= ymin ; y-=dy){
            //double y = ymax;
            int p1,p2;
            double xl,xr,zl,zr;
            double px[2];
            double pz[2];
            int p = 0;
//            if(ymax_point == 0){p1 = 1;p2 = 2;}
//            else if(ymax_point == 1){p1 = 0;p2 = 2;}
//            else if(ymax_point == 2){p1 = 0;p2 = 1;}
//
            double x1 = triangles[k].points[0][0];
            double y1 = triangles[k].points[0][1];
            double z1 = triangles[k].points[0][2];
            double x2 = triangles[k].points[1][0];
            double y2 = triangles[k].points[1][1];
            double z2 = triangles[k].points[1][2];
            double x3 = triangles[k].points[2][0];
            double y3 = triangles[k].points[2][1];
            double z3 = triangles[k].points[2][2];

//            if(y == ymax){xl = triangles[k].points[ymax_point][0]; xr = triangles[k].points[ymax_point][0];}
//            else if(y == y1 && y == y2){xl = x1; xr = x2; zl = z1; zr = z2;}
//            else if(y == y2 && y == y3){xl = x2; xr = x3; zl = z2; zr = z3;}
//            else if(y == y3 && y == y1){xl = x3; xr = x1; zl = z3; zr = z1;}
//            else if(y == y1){
//                xl = x1;
//                xr = x3 + ((y-y3)*(x2-x3))/(y2-y3);
//                zl = z1;
//                zr = z3 + ((y-y3)*(z2-z3))/(y2-y3);
//            }
//            else if(y == y2){
//                xl = x2;
//                xr = x1 + ((y-y1)*(x3-x1))/(y3-y1);
//                zl = z2;
//                zr = z1 + ((y-y1)*(z3-z1))/(y3-y1);
//            }
//            else if(y == y3){
//                xl = x3;
//                xr = x1 + ((y-y1)*(x2-x1))/(y2-y1);
//                zl = z3;
//                zr = z1 + ((y-y1)*(z2-z1))/(y2-y1);
//            }
//            else{
                if((y >= y1 && y<= y2) || (y <= y1 && y>= y2)){
                    px[p] = x1 + ((y-y1)*(x2-x1))/(y2-y1);
                    pz[p] = z1 + ((y-y1)*(z2-z1))/(y2-y1);
                    p++;
                }
                if((y >= y1 && y<= y3) || (y <= y1 && y>= y3)){
                    px[p] = x1 + ((y-y1)*(x3-x1))/(y3-y1);
                    pz[p] = z1 + ((y-y1)*(z3-z1))/(y3-y1);
                    p++;
                }
                if((y >= y3 && y<= y2) || (y <= y3 && y>= y2)){
                    px[p] = x3 + ((y-y1)*(x2-x3))/(y2-y3);
                    pz[p] = z3 + ((y-y1)*(z2-z3))/(y2-y3);
                    p++;
                }
            //}

            xl = px[0];
            xr = px[1];
            zl = pz[0];
            zr = pz[1];

            if(xl > xr){
                double temp = xl;
                xl = xr;
                xr = temp;

                zl = temp;
                zr = zl;
                zl = temp;
            }

            cout<<"p xl xr "<<p<<" "<<xl<<" "<<xr<<"\n";


//            double scan_point_1 = x3 + ((y3-y)*(x3-x1))/(y3-y1);
//            double scan_point_2 = x3 + ((y3-y)*(x3-x2))/(y3-y2);
//            cout<<"Scan Line : "<< scan_point_1 << "  " << scan_point_2<<"\n";
//
//            double zp1 = z3 + ((y3-y)*(z3-z1))/(y3-y1);
//            double zp2 = z3 + ((y3-y)*(z3-z2))/(y3-y2);
//            cout<<"Z Line : "<< zp1 << "  " << zp2<<"\n";
//
//            if(scan_point_1 > scan_point_2){
//                xl = scan_point_2;
//                xr = scan_point_1;
//                zl = zp2;
//                zr = zp1;
//            }
//            else{
//                xl = scan_point_1;
//                xr = scan_point_2;
//                zl = zp1;
//                zr = zp2;
//            }

            ///clipping
            if(xl < x_limit)xl = x_limit;//////////////
            if(xr > -x_limit)xr = -x_limit;

//            if(zl < z_limit[0])zl = z_limit[0];
//            if(zl > z_limit[1])zl = z_limit[1];
//            if(zr < z_limit[0])zr = z_limit[0];
//            if(zr > z_limit[1])zr = z_limit[1];

            int r = round((top_y - y)/dy);

            for(double x = xl ; x <= xr; x+=dx){
                int c = round((x - left_x)/dx);
                double z = zl + ((x-xl)*(zr-zl))/(xr-xl);
                if(z < z_buffer[r][c]){
                    z_buffer[r][c] = z;
                    image.set_pixel(c,r,triangles[k].color[0],triangles[k].color[1],triangles[k].color[2]);
                }
            }

        }

    }
   image.save_image("test1.bmp");
//    image.clear();


    return 0;
}
