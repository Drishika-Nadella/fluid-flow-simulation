// Viewing, camera and ilumination parameters
int main_window;
int winWidth, winHeight; // size of the graphics window, in pixels
// Perspective projection parameters
float fov = 80;
float aspect = 1;
float z_near = 0.1;
float z_far = 2000;
// Modelview (camera extrinsic) parameters
float eye_x = 650, eye_y = -220, eye_z = 380;
float c_x   = 650, c_y   = 300,  c_z   = 0;
float up_x  = 0,  up_y  = 0,  up_z  = 1;

// Simulation control variables
int frozen = 0; //toggles on/off the animation

// Colormapping variables
int clamp_flag = 0;
float clamp_min = 0, clamp_max = 0.2;
int scaling_flag = 0;
float dataset_min = 0, dataset_max = 10;
int quantize_colormap = 0;
int draw_matter_colorbar = 1;
int draw_isolines_colorbar = 0;
double colorbar_min = 0;
double colorbar_max = 1;
ColorMap fire = ColorMap((char*)"Fire");
ColorMap rainbow = ColorMap((char*)"Rainbow");
ColorMap custom = ColorMap((char*)"Custom");
int custom_color_index = 0;
float **custom_color_ranges = (float**) malloc(5*(sizeof(float*))); // up to 5 interpolations on a custom colormap
unsigned int matter_texture[1];
unsigned int isoline_texture[1];

// Matter visualization variables
int draw_matter = 1; // draw the smoke (matter) or not
int scalar_colormap = COLOR_RAINBOW; // method for scalar coloring
int matter_dataset = SCALAR_RHO;

// Glyphs visualization variables
int draw_glyphs_flag = 0; //draw the vector field or not
int color_dir = 0; //use direction color-coding or not
float vec_scale = 1000;	//scaling of hedgehogs
Glyphs glyphs = Glyphs();

// Isolines variables
int draw_isolines_flag = 0;
int isoline_colormap = COLOR_CUSTOM;
IsolineManager isoline_manager = IsolineManager();

// Height Plot variables
int draw_height_flag = 0;
int height_dataset_coloring = SCALAR_RHO;
float dataset_scale = 30;

// Stream Tubes variables
int draw_st_flag = 0;
StreamTubeManager stream_tube_manager = StreamTubeManager();
int st_radius = SCALAR_RHO;
float st_height = 10;

void update_textures()
{
	// Matter colormap
	glGenTextures(1, matter_texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glBindTexture(GL_TEXTURE_1D,matter_texture[0]);
	int size;
	if (quantize_colormap == 0)
		size = 256;
	else
		size = quantize_colormap;

	float textureImage[3*size];
	for(int j=0;j<size;++j)
	{
		float v = float(j)/(size-1);
		Color c;
		switch(scalar_colormap)
	    {
	    case COLOR_BLACKWHITE:
			c = Color(v,v,v);
			break;
	    case COLOR_RAINBOW:
	        c = rainbow.get_color(v);
	        break;
	    case COLOR_FIRE:
	        c = fire.get_color(v);
	        break;
	    case COLOR_CUSTOM:
	        c = custom.get_color(v);
	        break;
	    }
		textureImage[3*j]   = c.r;
		textureImage[3*j+1] = c.g;
		textureImage[3*j+2] = c.b;
	}
	glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,size,0,GL_RGB,GL_FLOAT,textureImage);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_1D,matter_texture[0]);

	// Isoline colormap
	glGenTextures(1, isoline_texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glBindTexture(GL_TEXTURE_1D,isoline_texture[0]);
	for(int j=0;j<size;++j)
	{
		float v = float(j)/(size-1);
		Color c;
		switch(isoline_colormap)
	    {
	    case COLOR_BLACKWHITE:
			c = Color(v,v,v);
			break;
	    case COLOR_RAINBOW:
	        c = rainbow.get_color(v);
	        break;
	    case COLOR_FIRE:
	        c = fire.get_color(v);
	        break;
	    case COLOR_CUSTOM:
	        c = custom.get_color(v);
	        break;
	    }
		textureImage[3*j]   = c.r;
		textureImage[3*j+1] = c.g;
		textureImage[3*j+2] = c.b;
	}
	glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,size,0,GL_RGB,GL_FLOAT,textureImage);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_1D,isoline_texture[0]);
}


void init_colormaps()
{
    // Example of how to "build" a colormap
    fire.add_color_range(Color(0,0,0), Color(1,0,0), 0, 0.5);
    fire.add_color_range(Color(1,0,0), Color(1,1,0), 0.5, 0.9);
	fire.add_color_range(Color(1,1,0), Color(1,1,1), 0.9, 1);
    fire.add_color_range(Color(1,1,1), Color(1,1,1), 1, 10);

    update_textures();
}

//set_color: Sets three different types of colormaps
void set_color(double v, int colormap)
{
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


void draw_colorbar()
{
    double n_samples = 256;

    for (int i = 0; i < n_samples; i++)
    {
        if (clamp_flag)
        {
            colorbar_max = clamp_max;
            colorbar_min = clamp_min;
        }
        else
        {
            colorbar_max = 1;
            colorbar_min = 0;
        }
        colorbar_max *=1.1;

        double current_value = colorbar_min + (i/n_samples)*(colorbar_max-colorbar_min);

		if (draw_matter_colorbar)
		{
			glEnable(GL_TEXTURE_1D);
			glBindTexture(GL_TEXTURE_1D,matter_texture[0]);
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
			set_color(current_value, scalar_colormap);
			glRectd(0.955*winWidth, i*((winHeight-80)/n_samples)+40,
					0.965*winWidth, (i+1)*((winHeight-80)/n_samples)+40);

			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
			glDisable(GL_TEXTURE_1D);
		}

		if (draw_isolines_colorbar)
		{
			glEnable(GL_TEXTURE_1D);
			glBindTexture(GL_TEXTURE_1D,isoline_texture[0]);
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
			set_color(current_value, scalar_colormap);
			glRectd(0.94*winWidth, i*((winHeight-80)/n_samples)+40,
					0.95*winWidth, (i+1)*((winHeight-80)/n_samples)+40);

			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
			glDisable(GL_TEXTURE_1D);
		}

		if (draw_isolines_colorbar || draw_matter_colorbar)
		{
			if(i % ((int)n_samples/10) == 0)
			{
				std::string str = std::to_string(current_value);
				glMatrixMode( GL_MODELVIEW );
				glPushMatrix();
				glLoadIdentity();
				//glColor3f(1,1,1);
				glRasterPos2i( 0.975*winWidth, i*((winHeight-80)/n_samples)+40);  // move in 10 pixels from the left and bottom edges
				for (unsigned j = 0; j < 5; ++j ) //only first 5 characters
				{
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
				}
				glPopMatrix();
				glMatrixMode( GL_PROJECTION );
				glPopMatrix();
				glMatrixMode( GL_MODELVIEW );
			}
		}
    }
}


void draw_isolines(double *dataset)
{
    double wn = (double)(winWidth*0.95) / (double)(DIM + 1);   // Grid cell width
    double hn = (double)(winHeight) / (double)(DIM + 1);  // Grid cell heigh
	glLineWidth(3.0);
	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, isoline_texture[0]);


    for(vector<Isoline>::iterator iso_it = isoline_manager.isoline_vector.begin(); iso_it != isoline_manager.isoline_vector.end(); ++iso_it)
    {
        for(vector<pair<float, float> >::iterator points_it = iso_it->points.begin(); points_it != iso_it->points.end(); points_it+=2)
        {
            float p1x = wn + (float)points_it->second * wn;
            float p1y = hn + (float)points_it->first * hn;

            float p2x = wn + (float)(points_it+1)->second * wn;
            float p2y = hn + (float)(points_it+1)->first * hn;

            glBegin(GL_LINES);
            set_color(iso_it->v, isoline_colormap);
            glVertex2f(p1x,p1y);
            glVertex2f(p2x,p2y);
            glEnd();
       }
    }
	glDisable(GL_TEXTURE_1D);
	glLineWidth(1.0);
}

void compute_normal(int idx, float *norm)
{
    float v_lenght = sqrtf(pow(der_sfx[idx]*dataset_scale,2)
                        + pow(der_sfy[idx]*dataset_scale,2) + 1);
    if (v_lenght < 1.0e-6)
        v_lenght = 1;

    norm[0] = (der_sfx[idx]*dataset_scale)/v_lenght;
    norm[1] = (der_sfy[idx]*dataset_scale)/v_lenght;
    norm[0] = 1.0/v_lenght;
}

void cross_product(float ax, float ay, float az, float bx, float by, float bz, float *rx, float *ry, float *rz)
{
	float v_lenght = sqrtf(pow(ax-bx,2) + pow(ay-by,2) + pow(az-bz,2));
	if (v_lenght < 1.0e-6)
		v_lenght = 1;

	*rx = (ay*bz - az*by)/v_lenght;
	*ry = (az*bx - ax*bz)/v_lenght;
	*rz = (ax*by - ay*bx)/v_lenght;
}

void draw_stream_tubes()
{
    fftw_real wn = (fftw_real)(winWidth*0.95) / (fftw_real)(DIM + 1);   // Grid cell width
    fftw_real hn = (fftw_real)(winHeight) / (fftw_real)(DIM + 1);  // Grid cell heigh

	int points = 20;
	double slice = 2*M_PI/points;

	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, isoline_texture[0]);

    for(unsigned tube = 0; tube < stream_tube_manager.stream_tube_vector.size(); ++tube)
	{
        stream_tube_manager.stream_tube_vector[tube].calc_all_points(100, st_radius);
		vector<tuple<float, float, float> > v = stream_tube_manager.stream_tube_vector[tube].stream_tube_points;

        for (unsigned i = 0; i < stream_tube_manager.stream_tube_vector[tube].stream_tube_points.size()-1; ++i)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int k = 0; k < points; ++k)
			{
				double angle = slice * k;
				float radius1 = std::get<2>(v[i]);
				float radius2 = std::get<2>(v[i+1]);

				float x1 = wn*std::get<0>(v[i]) + radius1*cos(angle);
				float y1 = hn*std::get<1>(v[i]) + radius1*sin(angle);
				float z1 = i*st_height;
				float x2 = wn*std::get<0>(v[i+1]) + radius2*cos(angle);
				float y2 = hn*std::get<1>(v[i+1]) + radius2*sin(angle);
				float z2 = (i+1)*st_height;

				angle = slice * (k+1);
				float x3 = wn*std::get<0>(v[i]) + radius1*cos(angle);
				float y3 = hn*std::get<1>(v[i]) + radius1*sin(angle);
				float z3 = i*st_height;
				float x4 = wn*std::get<0>(v[i+1]) + radius2*cos(angle);
				float y4 = hn*std::get<1>(v[i+1]) + radius2*sin(angle);
				float z4 = (i+1)*st_height;

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				set_color(0, isoline_colormap);
				float nx, ny, nz;
				cross_product(x1-x2, y1-y2, z1-z2, x1-x3, y1-y3, z1-z3, &nx, &ny, &nz);
				glNormal3f(nx, ny, nz);
				glVertex3f(x3, y3, z3);
				glVertex3f(x4, y4, z4);
		        glVertex3f(x1, y1, z1);
		        glVertex3f(x2, y2, z2);
			}

			glEnd();
        }
	}
	glDisable(GL_TEXTURE_1D);
}

//visualize: This is the main visualization function
void visualize(void)
{
    fftw_real wn = (fftw_real)(winWidth*0.95) / (fftw_real)(DIM + 1);   // Grid cell width
    fftw_real hn = (fftw_real)(winHeight) / (fftw_real)(DIM + 1);  // Grid cell heigh

    fftw_real *dataset;
    if (matter_dataset == SCALAR_RHO)
        dataset = rho;
    else if (matter_dataset == SCALAR_VELOC_MAG)
        dataset = v_mag;
    else if (matter_dataset == SCALAR_FORCE_MAG)
        dataset = f_mag;
    else if (matter_dataset == SCALAR_FORCE_DIV || matter_dataset == SCALAR_VELOC_DIV)
        dataset = div_vf;

    // using other scalar field for colormapping
    fftw_real *color_dataset;
    if (height_dataset_coloring == SCALAR_RHO)
        color_dataset = rho;
    else if (height_dataset_coloring == SCALAR_VELOC_MAG)
        color_dataset = v_mag;
    else if (height_dataset_coloring == SCALAR_FORCE_MAG)
        color_dataset = f_mag;
    else if (height_dataset_coloring == SCALAR_FORCE_DIV || height_dataset_coloring == SCALAR_VELOC_DIV)
        color_dataset = div_vf;



    if (draw_matter)
    {
		glEnable(GL_TEXTURE_1D);
		glBindTexture(GL_TEXTURE_1D, matter_texture[0]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL); // Tell OpenGL to use the values passed by glColor() as material properties
        for (int j = 0; j < DIM - 1; j++)
        {
            double px,py;
            glBegin(GL_TRIANGLE_STRIP);

            int i = 0;
            px = wn + (fftw_real)i * wn;
            py = hn + (fftw_real)j * hn;
            int idx = (j * DIM) + i;
            set_color(dataset[idx], scalar_colormap);
            glVertex3f(px,py,-0.01);

            for (i = 0; i < DIM - 1; i++)
            {
                px = wn + (fftw_real)i * wn;
                py = hn + (fftw_real)(j + 1) * hn;
                idx = ((j + 1) * DIM) + i;
                set_color(dataset[idx], scalar_colormap);
                glVertex3f(px,py,-0.01);
                px = wn + (fftw_real)(i + 1) * wn;
                py = hn + (fftw_real)j * hn;
                idx = (j * DIM) + (i + 1);
                set_color(dataset[idx], scalar_colormap);
                glVertex3f(px,py,-0.01);
            }
            px = wn + (fftw_real)(DIM - 1) * wn;
            py = hn + (fftw_real)(j + 1) * hn;
            idx = ((j + 1) * DIM) + (DIM - 1);
            set_color(dataset[idx], scalar_colormap);
            glVertex3f(px,py,-0.01);
            glEnd();
        }
		glDisable(GL_TEXTURE_1D);
    }

    if (draw_height_flag)
    {
        // Set "camera"
		glEnable(GL_TEXTURE_1D);
		glBindTexture(GL_TEXTURE_1D, matter_texture[0]);
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye_x,eye_y,eye_z,c_x,c_y,c_z,up_x,up_y,up_z);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov, float(winWidth)/winHeight, z_near, z_far);

        glShadeModel(GL_SMOOTH);
        glEnable(GL_COLOR_MATERIAL); // Tell OpenGL to use the values passed by glColor() as material properties

        GLfloat light0_ambient[] =  {.2f, 0.2f, 0.2f, 1.0f};
        GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
        GLfloat light0_position[] = {500.0f, 500.0f, 10.0f, 0.0f};
        GLfloat mat_shininess[] = { 90.0 };

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

        float norm [3];

        float clamp_max = 150;

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (int j = 0; j < DIM - 1; j++)
        {
            double px,py,pz;
            glBegin(GL_TRIANGLE_STRIP);

            int i = 0;
            px = wn + (fftw_real)i * wn;
            py = hn + (fftw_real)j * hn;
            int idx = (j * DIM) + i;
            set_color(color_dataset[idx], scalar_colormap);
            compute_normal(idx, norm);
            glNormal3f(norm[0], norm[1], norm[2]);
            pz = dataset[idx]*dataset_scale;
            if (pz > clamp_max) pz = clamp_max;
            glVertex3f(px, py, pz);

            for (i = 0; i < DIM - 1; i++)
            {
                px = wn + (fftw_real)i * wn;
                py = hn + (fftw_real)(j + 1) * hn;
                idx = ((j + 1) * DIM) + i;
                set_color(color_dataset[idx], scalar_colormap);
                compute_normal(idx, norm);
                glNormal3f(norm[0], norm[1], norm[2]);
                pz = dataset[idx]*dataset_scale;
                if (pz > clamp_max) pz = clamp_max;
                glVertex3f(px, py, pz);
                px = wn + (fftw_real)(i + 1) * wn;
                py = hn + (fftw_real)j * hn;
                idx = (j * DIM) + (i + 1);
                set_color(color_dataset[idx], scalar_colormap);
                compute_normal(idx, norm);
                glNormal3f(norm[0], norm[1], norm[2]);
                pz = dataset[idx]*dataset_scale;
                if (pz > clamp_max) pz = clamp_max;
                glVertex3f(px, py, pz);
            }
            px = wn + (fftw_real)(DIM - 1) * wn;
            py = hn + (fftw_real)(j + 1) * hn;
            idx = ((j + 1) * DIM) + (DIM - 1);
            set_color(color_dataset[idx], scalar_colormap);
            compute_normal(idx, norm);
            glNormal3f(norm[0], norm[1], norm[2]);
            pz = dataset[idx]*dataset_scale;
            if (pz > clamp_max) pz = clamp_max;
            glVertex3f(px, py, pz);
            glEnd();
        }
         glDisable(GL_TEXTURE_1D);
    }

	if (draw_matter_colorbar)
		draw_colorbar();

	if (draw_glyphs_flag)
        glyphs.draw_glyphs();

    if (draw_isolines_flag)
    {
        if(isoline_manager.isoline_vector.empty())
            isoline_manager.reset();
        isoline_manager.compute_isolines();
        draw_isolines(dataset);
    }

    if (draw_st_flag)
    {
        draw_stream_tubes();
    }
}


//display: Handle window redrawing events. Simply delegates to visualize().
void display(void)
{
	glClearColor(0,0,0,0); // Clear the frame and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
    visualize();
    glFlush();
    glutSwapBuffers();
}

//reshape: Handle window resizing (reshaping) events
void reshape(int w, int h)
{
    glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    winWidth = w; winHeight = h;
	c_x = eye_x = (winWidth*0.95)/2;
}
