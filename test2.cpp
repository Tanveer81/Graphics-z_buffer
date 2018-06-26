double xmax = triangles[k].points[0][1];
        int xmax_point = 0;
        double xmin = triangles[k].points[0][1];
        int xmin_point = 0;
        for (int i = 1; i < 3; i++){
            if(triangles[k].points[i][1] > xmax){
                xmax = triangles[k].points[i][1];
                xmax_point = i;
            }
            if(triangles[k].points[i][1] < xmin){
                xmin = triangles[k].points[i][1];
                xmin_point = i;
            }
        }

        cout<<xmax<<"  "<<xmin<<"\n";
