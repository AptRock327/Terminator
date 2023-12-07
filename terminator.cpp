// "TERMINATOR" - Terminal-based 3D rendering and movement engine 1.0 by Dominik Śliwiński

#include <iostream>
#include <vector>
#include <math.h> //for trig

//buffer info
const int width = 60;
const int height = 60;

//camera position
int camera_x = 0;
int camera_y = 0;
float camera_z = 0;

float theta_x = 0; //horizontal camera rotation
float theta_y = 0; //vertical camera rotation

//graphics buffer
char buffer[width][height];

//cube vertices
std::vector<float> x_values{1, -1, 1, -1, 1, -1, 1, -1};
std::vector<float> y_values{1, 1, -1, -1, 1, 1, -1, -1};
std::vector<float> z_values{3, 3, 3, 3, 4, 4, 4, 4};

//In this project, the simplest kind of projection is used: basic perspective projection.
//As we know, the further away a given object is (the larger its z value), the closer it is to the origin (0, 0).
//Hence, we can simply divide both the x and y coordinate by z (this is done for specific coordinates separately later).
std::vector<float> projection(std::vector<float> coord, std::vector<float> z)
{
    std::vector<float> output(coord.size());
    for(int i = 0; i < z.size(); i++)
    {
       output[i] = (float)coord[i]/((float)z[i]+camera_z);
    }
    return output;
}

//These functions are responsible for calculating the x and y positions of a given coordinate after a 2D rotation.
//We recall that for any pair of coordinates (x, y) there exists an r and alpha such that x = r cos(alpha) and y = r sin(alpha).
//Alpha is then the angle on a circle with radius r necessary to reach said points.
//To rotate the pair of coordinates, we consider tilting them along this circle by an angle theta.
//We observe that x' = r cos(alpha + theta) = r (cos(alpha)cos(theta) - sin(alpha)sin(theta)) = r cos(alpha)cos(theta) - r sin(alpha)sin(theta).
//Furthermore, y' = r sin(alpha + theta) = r (sin(alpha)cos(theta) + sin(theta)cos(alpha)) = r sin(alpha)cos(theta) + r sin(theta)cos(alpha).
//Remembering that r cos(alpha) is x, and r sin(alpha) is y, we realize that x' = x cos(theta) - y sin(theta) and y' = y cos(theta) + x sin(theta).
//We use this to produce the x and y coordinates after a rotation by an angle theta.

std::vector<float> rotate_x(std::vector<float> x, std::vector<float> y, float theta)
{
    std::vector<float> output(x.size());
    for(int i = 0; i < x.size(); i++)
    {
        output[i] = x[i] * cos(theta) - y[i] * sin(theta);
    }
    return output;
}

std::vector<float> rotate_y(std::vector<float> x, std::vector<float> y, float theta)
{
    std::vector<float> output(x.size());
    for(int i = 0; i < x.size(); i++)
    {
        output[i] = x[i] * sin(theta) + y[i] * cos(theta);
    }
    return output;
}

int main()
{
    int a = 0; //The a and b variables will hold the result of a certain computation. Creating variables is faster than recalculating.
    int b = 0;
    char move; //This variable will hold the player's input.
    //Rotated coordinates (z coordinates will be rotated twice)
    std::vector<float> x_rotated;
    std::vector<float> y_rotated;
    std::vector<float> z_rotated;
    std::vector<float> z_rotated_again;
    //Projected coordinates
    std::vector<float> x_projected;
    std::vector<float> y_projected;

    //Main loop
    while(true)
    {
        //The player can rotate the camerally horizontally.
        //One must realize that rotating everything in a top-down view (where y is constant) is equivalent to horizontal rotation in 3D.
        //Hence, we can rotate horizontally by rotating the x and z values.
        x_rotated = rotate_x(x_values, z_values, theta_x);
        z_rotated = rotate_y(x_values, z_values, theta_x);

        //Then, we can rotate vertically by rotating the y and z values with a similar intuition (we imagine from the side where x is constant).
        y_rotated = rotate_x(y_values, z_rotated, theta_y);
        z_rotated_again = rotate_y(y_values, z_rotated, theta_y);

        //The values are projected.
        x_projected = projection(x_rotated, z_rotated_again);
        y_projected = projection(y_rotated, z_rotated_again);

        //The buffer is cleared every move in order to draw the new image onto it.
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                buffer[i][j] = ' ';
            }
        }

        //The values are multiplied by the height or width to scale the normalized (x,y) values onto the whole screen.
        //We want the origin (0,0) to be in the middle.
        //Hence, the heigth and width have been divided by 2 to scale values only onto a half as they're negative or positive already.
        //To finish keeping the origin in the middle, the width and height divided the two are also added.
        //Then, the value of 'k' is inserted to this point of a buffer if the operation is legal.
        for(int i = 0; i < x_projected.size(); i++)
        {
            a = (int)(x_projected[i]*(float)width/2)+width/2+camera_x;
            b = (int)(y_projected[i]*(float)height/2)+height/2+camera_y;
            if(a >= 0 && b >= 0 && a < width && b < height) buffer[a][b] = 'k';
        }

        //output the buffer into the terminal
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                std::cout << buffer[j][i];
            }
            std::cout << std::endl;
        }

        move = getchar(); //get character corresponding to the player's move

        //possible player movements
        if(move == 'd') camera_x-=5;
        if(move == 'a') camera_x+=5;
        if(move == ' ') camera_y+=5;
        if(move == 'q') camera_y-=5;
        if(move == 'w') camera_z-=0.5;
        if(move == 's') camera_z+=0.5;
        if(move == 'l') theta_x+=0.1;
        if(move == 'j') theta_x-=0.1;
        if(move == 'i') theta_y+=0.1;
        if(move == 'k') theta_y-=0.1;
    }
    return 0;
}
