#include "Glyphs.h"
#define PI 3.14159265

Glyphs::Glyphs()
{
    glyph_type = GLYPH_ARROW;
    scalar_field = SCALAR_RHO;
    vector_field = VECTOR_VELOC;
    vec_scale = 1300;
    x_axis_samples = 15;
    y_axis_samples = 15;
    vf_x = vx;
    vf_y = vy;
}

void Glyphs::draw_glyphs()
{
    double wn = (double)(winWidth*0.95) / (double)(DIM + 1);   // Grid cell width
    double hn = (double)(winHeight) / (double)(DIM + 1);  // Grid cell heigh

    if (vector_field == VECTOR_VELOC)
    {
        vf_x = vx;
        vf_y = vy;
    }
    else if (vector_field == VECTOR_FORCE)
    {
        vf_x = fx;
        vf_y = fy;
    }

    switch (scalar_field) {
    case SCALAR_RHO:
    case SCALAR_VELOC_MAG:
    case SCALAR_FORCE_MAG:
        glEnable(GL_TEXTURE_1D);
        break;
    }

    if (glyph_type == GLYPH_LINE)
    {
        glBegin(GL_LINES);
        for (double i = 0; i < DIM+0.01; i += (DIM/(double)x_axis_samples))
        {
            for (double j = 0; j < DIM+0.01; j += (DIM/(double)y_axis_samples))
            {
                double vec [2] = {0,0};
                srand (i*DIM +j);
                double ni = abs(fmod(i + jitter*3*sin(rand()), DIM-1));
                double nj = abs(fmod(j + jitter*3*cos(rand()), DIM-1));

                color_glyph(round(ni), round(nj)); // nearest neighbour policy for coloring
                bilinear_interpolation(vec, vf_x, vf_y, ni, nj); // interpolation for vetor values
                glVertex2f(wn + ni * wn, hn + nj * hn);
                glVertex2f((wn + ni * wn) + vec_scale * vec[0],
                           (hn + nj * hn) + vec_scale * vec[1]);
            }
        }
        glEnd();
    }
    else if (glyph_type == GLYPH_NEEDLE)
    {
        glBegin(GL_TRIANGLES);

        for (double i = 0; i < DIM+0.01; i += (DIM/(double)x_axis_samples))
            for (double j = 0; j < DIM+0.01; j += (DIM/(double)y_axis_samples))
            {
                double vec [2] = {0,0};
                srand (i*DIM +j);
                double ni = abs(fmod(i + jitter*3*sin(rand()), DIM-1));
                double nj = abs(fmod(j + jitter*3*cos(rand()), DIM-1));

                color_glyph(round(ni), round(nj)); // nearest neighbour polocy for coloring
                bilinear_interpolation(vec, vf_x, vf_y, ni, nj); // interpolation for vetor values

                glVertex2f(wn + ni*wn, hn + nj*hn);
                glVertex2f((wn + ni*wn) + vec_scale * vec[0],
                           (hn + nj*hn) + vec_scale * vec[1]);

                double angle_rad = 0.0;
                glVertex2f((wn + ni*wn) + 0.05*vec_scale*(cos(angle_rad)*vec[1] -sin(angle_rad)*vec[0]),
                            (hn + nj*hn) + 0.05*vec_scale*(sin(angle_rad)*vec[1] -cos(angle_rad)*vec[0]));

                glVertex2f(wn + ni*wn, hn + nj*hn);
                glVertex2f((wn + ni*wn) + vec_scale * vec[0],
                           (hn + nj*hn) + vec_scale * vec[1]);
                glVertex2f((wn + ni*wn) + (-0.05)*vec_scale*(cos(angle_rad)*vec[1] -sin(angle_rad)*vec[0]),
                           (hn + nj*hn) + (-0.05)*vec_scale*(sin(angle_rad)*vec[1] -cos(angle_rad)*vec[0]));

            }
        glEnd();
    }
    else if (glyph_type == GLYPH_ARROW)
    {
        glBegin(GL_LINES);
        for (double i = 0; i < DIM+0.01; i += (DIM/(double)x_axis_samples))
        {
            for (double j = 0; j < DIM+0.01; j += (DIM/(double)y_axis_samples))
            {
                double vec [2] = {0,0};
                srand (i*DIM +j);
                double ni = abs(fmod(i + jitter*3*sin(rand()), DIM-1));
                double nj = abs(fmod(j + jitter*3*cos(rand()), DIM-1));
                color_glyph(round(ni), round(nj)); // nearest neighbour polocy for coloring
                bilinear_interpolation(vec, vf_x, vf_y, ni, nj); // interpolation for vetor values
                glVertex2f(wn + ni * wn, hn + nj * hn);
                glVertex2f((wn + ni * wn) + vec_scale * vec[0],
                           (hn + nj * hn) + vec_scale * vec[1]);
            }
        }
        glEnd();

        glBegin(GL_TRIANGLES);
        for (double i = 0; i < DIM+0.01; i += (DIM/(double)x_axis_samples))
            for (double j = 0; j < DIM+0.01; j += (DIM/(double)y_axis_samples))
            {
                double vec [2] = {0,0};
                srand (i*DIM +j);
                double ni = abs(fmod(i + jitter*3*sin(rand()), DIM-1));
                double nj = abs(fmod(j + jitter*3*cos(rand()), DIM-1));
                color_glyph(round(ni), round(nj)); // nearest neighbour polocy for coloring
                bilinear_interpolation(vec, vf_x, vf_y, ni, nj); // interpolation for vetor values

                glVertex2f((wn + ni*wn) + vec_scale * vec[0],
                            (hn + nj*hn) + vec_scale * vec[1]);

                double angle_rad = 0.0;
                glVertex2f((wn + ni*wn) + 0.8*vec_scale*vec[0] + 0.05*vec_scale*(cos(angle_rad)*vec[1] -sin(angle_rad)*vec[0]),
                           (hn + nj*hn) + 0.8*vec_scale*vec[1] + 0.05*vec_scale*(sin(angle_rad)*vec[1] -cos(angle_rad)*vec[0]));

                glVertex2f((wn + ni*wn) + 0.8*vec_scale*vec[0] + (-0.05)*vec_scale*(cos(angle_rad)*vec[1] -sin(angle_rad)*vec[0]),
                           (hn + nj*hn) + 0.8*vec_scale*vec[1] + (-0.05)*vec_scale*(sin(angle_rad)*vec[1] -cos(angle_rad)*vec[0]));

            }
            glEnd();
    }

    switch (scalar_field) {
    case SCALAR_RHO:
    case SCALAR_VELOC_MAG:
    case SCALAR_FORCE_MAG:
        glDisable(GL_TEXTURE_1D);
        break;
    }
}

void Glyphs::bilinear_interpolation(double *vec, double *vf_x, double *vf_y, double i, double j)
{
    double x_fxy1 = vf_x[int(floor(j)*DIM + floor(i))]*(1 - (i-floor(i)))
                + vf_x[int(floor(j)*DIM + ceil(i))]*(i-floor(i));

    double x_fxy2 = vf_x[int(ceil(j)*DIM + floor(i))]*(1 - (i-floor(i)))
                + vf_x[int(ceil(j)*DIM + ceil(i))]*(i-floor(i));

    double x_fxy = x_fxy1*(1-(j-floor(j))) + x_fxy2*(j-floor(j));


    double y_fxy1 = vf_y[int(floor(j)*DIM + floor(i))]*(1 - (i-floor(i)))
                + vf_y[int(floor(j)*DIM + ceil(i))]*(i-floor(i));

    double y_fxy2 = vf_y[int(ceil(j)*DIM + floor(i))]*(1 - (i-floor(i)))
                + vf_y[int(ceil(j)*DIM + ceil(i))]*(i-floor(i));

    double y_fxy = y_fxy1*(1-(j-floor(j))) + y_fxy2*(j-floor(j));

    vec[0] = x_fxy;
    vec[1] = y_fxy;

}


void Glyphs::color_glyph(int i, int j)
{
    int idx = (j * DIM) + i;
    double v;

    switch (scalar_field) {
    case SCALAR_WHITE:
        glColor3f(1,1,1);
        return;
    case SCALAR_DIR:
        direction_to_color(vf_x[idx],vf_y[idx]);
        return;
    case SCALAR_RHO:
        v = rho[idx];
        break;
    case SCALAR_VELOC_MAG:
        v = v_mag[idx];
        break;
    case SCALAR_FORCE_MAG:
        v = f_mag[idx];
        break;
    }

    double out_min = 0, out_max = 1; // considering that values on the simulation and visualization range 0-1 (which they don't!)
    if (clamp_flag)
    {
        if (v > clamp_max) v = clamp_max; if (v < clamp_min) v = clamp_min;
        // map interval clamp_min - clamp_max -> out_min - out_max
        v = (v - clamp_min) * (out_max - out_min) / (clamp_max - clamp_min) + out_min;
    }

    if (scaling_flag)
        v = (v - dataset_min) * (out_max - out_min) / (dataset_max - dataset_min) + out_min;

    glTexCoord1f(v);
}

void Glyphs::direction_to_color(float x, float y)
{
    float r,g,b,f;

    f = atan2(y,x) / 3.1415927 + 1;
    r = f;
    if(r > 1) r = 2 - r;
    g = f + .66667;
    if(g > 2) g -= 2;
    if(g > 1) g = 2 - g;
    b = f + 2 * .66667;
    if(b > 2) b -= 2;
    if(b > 1) b = 2 - b;

    glColor3f(r,g,b);
}
