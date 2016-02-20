// colormap button ids
#define CM_BW_ID 100
#define CM_RB_ID 101
#define CM_BD_ID 102
#define CM_FIRE_ID 103
#define CM_CUST_ID 104
// scalar field selector id on listbox
#define SF_RHO_ID 150
#define SF_VELOC_ID 151
#define SF_FORCE_ID 152
#define SF_DIR_ID 162
#define SF_WHITE_ID 163
// vector fields ids
#define VF_VELOC_ID 160
#define VF_FORCE_ID 161
// glyph type ids
#define GLYPH_LINE 170
#define GLYPH_ARROW 171
#define GLYPH_CONE 172
// button/checkbox/listbox ids
#define DT_INCREASE_ID 200
#define DT_DECREASE_ID 201
#define HH_INCREASE_ID 202
#define HH_DECREASE_ID 203
#define FV_INCREASE_ID 204
#define FV_DECREASE_ID 205
#define PP_ID 206
#define QT_ID 207
#define CLAMPING_ID 208
#define SCALING_ID 209
#define QUANT_ID 210
#define SX_INCREASE_ID 220
#define SX_DECREASE_ID 221
#define SY_INCREASE_ID 222
#define SY_DECREASE_ID 223

// statictext objects pointers are global because control_cb callback
// function can't handle arguments except int values
GLUI_StaticText *dt_text, *hh_text, *visc_text;
GLUI_StaticText *x_sample_text, *y_sample_text;

// parameters window and custom colormap window global pointers
GLUI *glui;
GLUI *cust_window;

//display: Handle window redrawing events. Simply delegates to visualize().
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
}

//keyboard: Handle key presses
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 't': dt -= 0.001; break;
    case 'T': dt += 0.001; break;
    case 'c': color_dir = 1 - color_dir; break;
    case 'S': vec_scale *= 1.2; break;
    case 's': vec_scale *= 0.8; break;
    case 'V': visc *= 5; break;
    case 'v': visc *= 0.2; break;
    case 'x': draw_smoke = 1 - draw_smoke;
        if (draw_smoke==0) draw_glyphs_flag = 1; break;
    case 'y': draw_glyphs_flag = 1 - draw_glyphs_flag;
        if (draw_glyphs_flag==0) draw_smoke = 1; break;
    case 'm': scalar_col++; if (scalar_col>COLOR_BANDS) scalar_col=COLOR_BLACKWHITE; break;
    case 'a': frozen = 1-frozen; break;
    case 'q': exit(0);
    }
}


// drag: When the user drags with the mouse, add a force that corresponds to the direction of the mouse
//       cursor movement. Also inject some new matter into the field at the mouse location.
void drag(int mx, int my)
{
    int xi,yi,X,Y;
    double  dx, dy, len;
    static int lmx=0,lmy=0;				//remembers last mouse location

    // Compute the array index that corresponds to the cursor location
    xi = (int)clamp((double)(DIM + 1) * ((double)mx / (double)winWidth));
    yi = (int)clamp((double)(DIM + 1) * ((double)(winHeight - my) / (double)winHeight));

    X = xi; Y = yi;

    if (X > (DIM - 1))  X = DIM - 1; if (Y > (DIM - 1))  Y = DIM - 1;
    if (X < 0) X = 0; if (Y < 0) Y = 0;

    // Add force at the cursor location
    my = winHeight - my;
    dx = mx - lmx; dy = my - lmy;
    len = sqrt(dx * dx + dy * dy);
    if (len != 0.0) {  dx *= 0.1 / len; dy *= 0.1 / len; }
    fx[Y * DIM + X] += dx;
    fy[Y * DIM + X] += dy;
    rho[Y * DIM + X] = 10.0f;
    lmx = mx; lmy = my;
}

// update_variables_config_window: Used to write on the statictext values
void update_variables_config_window()
{
    // Update variable values on the config window
    char buffer[50];
    sprintf(buffer, "= %.2f", dt);
    dt_text->set_text(buffer);
    sprintf(buffer, "= %.2f", glyphs.vec_scale);
    hh_text->set_text(buffer);
    sprintf(buffer, "= %.6f", visc);
    visc_text->set_text(buffer);
    sprintf(buffer, "X axis = %d", glyphs.x_axis_samples);
    x_sample_text->set_text(buffer);
    sprintf(buffer, "Y axis = %d", glyphs.y_axis_samples);
    y_sample_text->set_text(buffer);
}

// resume: Unfortunate solution to rendering glui windows
void resume (int t){ frozen = t;}

// add_range_to_custom_cm: Takes an index i that corresponds to the possition on
//                         the custom_color_ranges global variable and sets it's values
//                         to the custom colormap;
void add_range_to_custom_cm (int i)
{
    float *args = custom_color_ranges[i];

    Color a = Color(args[0], args[1], args[2]);
    Color b = Color(args[3], args[4], args[5]);

    custom.add_color_range(a, b, args[6], args[7]);
    resume(0);
    cust_window->close();
}

// handle_custom_colormap: Deals with the custom colormap window
void handle_custom_colormap()
{
    cust_window = GLUI_Master.create_glui("Custom Dataset");

    float *colormap_args = (float *) malloc(8*sizeof(float));
    custom_color_ranges[custom_color_index] = colormap_args; //warning -> aliasing
    for (int i = 0; i < 8; i++)
        colormap_args[i] = 0.0;

    cust_window->add_statictext("Color 1");
    cust_window->add_edittext("R", GLUI_EDITTEXT_FLOAT, &colormap_args[0]);
    cust_window->add_edittext("G", GLUI_EDITTEXT_FLOAT, &colormap_args[1]);
    cust_window->add_edittext("B", GLUI_EDITTEXT_FLOAT, &colormap_args[2]);

    cust_window->add_column(true);

    cust_window->add_statictext("Color 2");
    cust_window->add_edittext("R", GLUI_EDITTEXT_FLOAT, &colormap_args[3]);
    cust_window->add_edittext("G", GLUI_EDITTEXT_FLOAT, &colormap_args[4]);
    cust_window->add_edittext("B", GLUI_EDITTEXT_FLOAT, &colormap_args[5]);

    cust_window->add_column(true);

    cust_window->add_statictext("Interpolation Interval");
    cust_window->add_edittext("Start", GLUI_EDITTEXT_FLOAT, &colormap_args[6]);
    cust_window->add_edittext("End", GLUI_EDITTEXT_FLOAT, &colormap_args[7]);

    new GLUI_Button(cust_window, "Add interpolation to colormap",
                    custom_color_index, add_range_to_custom_cm);
    custom_color_index++;

    GLUI_Master.auto_set_viewport();
    cust_window->set_main_gfx_window(main_window);
}

// control_cb: Takes (almost) all callbacks from buttons, checkboxes, etc.
void control_cb(int control)
{
    // In order to glut have enough time to render the config window
    // the program needs to be frozen for 100ms so the config window
    // becomes target of the display function instead of the main
    if (control == PP_ID)
    {
        (frozen == 0)? frozen = 1: frozen = 0;
    }
    else
    {
        frozen = 1;
        glutTimerFunc(100, resume, 0);

        switch (control)
        {
        case DT_INCREASE_ID:
            dt += 0.01;
            break;
        case DT_DECREASE_ID:
            dt -= 0.01;
            break;
        case HH_INCREASE_ID:
            glyphs.vec_scale *= 1.1;
            break;
        case HH_DECREASE_ID:
            glyphs.vec_scale *= 0.9;
            break;
        case FV_INCREASE_ID:
            visc *= 1.2;
            break;
        case FV_DECREASE_ID:
            visc *= 0.8;
            break;
        case QT_ID:
            exit(0);
            break;
        case CM_CUST_ID:
            glutTimerFunc(100, resume, 1);
            handle_custom_colormap();
        case CM_BD_ID:
        case CM_RB_ID:
        case CM_BW_ID:
        case CM_FIRE_ID:
            scalar_col = control;
            break;
        case QUANT_ID:
            break;
        case SX_INCREASE_ID:
            glyphs.x_axis_samples++;
            break;
        case SX_DECREASE_ID:
            glyphs.x_axis_samples--;
            break;
        case SY_INCREASE_ID:
            glyphs.y_axis_samples++;
            break;
        case SY_DECREASE_ID:
            glyphs.y_axis_samples--;
            break;
        }

        update_variables_config_window();
        glutPostRedisplay();
        //printf("dt: %.2f   Hedgehog Scale: %0.2f   Fluid Viscosity:%.5f \n", dt, vec_scale, visc);
    }
}

// init_control_window: Creates and configures UI elements
void init_control_window()
{

    glui = GLUI_Master.create_glui( "GLUI" );

    // Simulation Parameters
    GLUI_Panel *simu_panel = new GLUI_Panel (glui, "Simulation Parameters");

    GLUI_Panel *dt_panel = new GLUI_Panel (simu_panel, "Time Step");
    dt_text = glui->add_statictext_to_panel(dt_panel, "");
    new GLUI_Button(dt_panel, "Increase", DT_INCREASE_ID, control_cb);
    new GLUI_Button(dt_panel, "Decrease", DT_DECREASE_ID, control_cb);

    glui->add_column_to_panel(simu_panel, false);

    GLUI_Panel *visc_panel = new GLUI_Panel (simu_panel, "Fluid Viscosity");
    visc_text = glui->add_statictext_to_panel(visc_panel, "");
    new GLUI_Button(visc_panel, "Increase", FV_INCREASE_ID, control_cb);
    new GLUI_Button(visc_panel, "Decrease", FV_DECREASE_ID, control_cb);

    // Matter Parameters
    GLUI_Panel *upper_visu_panel = new GLUI_Panel(glui, "Visualization Paramenters");
    glui->add_checkbox_to_panel(upper_visu_panel, "Display Matter", &draw_smoke, 0, control_cb);
    GLUI_Panel *matter_panel = new GLUI_Panel(upper_visu_panel, "");

    GLUI_Panel *dataset_panel = new GLUI_Panel (matter_panel, "Dataset Selection");
    GLUI_Listbox *matter_dataset_lb = glui->add_listbox_to_panel(dataset_panel, "", &dataset_id);
    matter_dataset_lb->add_item(SF_RHO_ID, "Fluid Density");
    matter_dataset_lb->add_item(SF_VELOC_ID, "Fluid Velocity Magnitude");
    matter_dataset_lb->add_item(SF_FORCE_ID, "Force Field Magnitude");

    GLUI_Panel *color_panel = new GLUI_Panel (matter_panel, "Color Mapping");
    new GLUI_Button(color_panel, "Black and White", CM_BW_ID, control_cb);
    new GLUI_Button(color_panel, "Rainbow", CM_RB_ID, control_cb);
    new GLUI_Button(color_panel, "Fire", CM_FIRE_ID, control_cb);
    new GLUI_Button(color_panel, "Custom", CM_CUST_ID, control_cb);
    glui->add_edittext_to_panel(color_panel, "Quantize:", GLUI_EDITTEXT_INT, &quantize_colormap);

    GLUI_Panel *clamp_ro = glui->add_panel_to_panel(matter_panel, "Options", true);
    glui->add_checkbox_to_panel(clamp_ro, "Clampling", &clamp_flag, 0, control_cb);
    glui->add_edittext_to_panel(clamp_ro, "MIN", GLUI_EDITTEXT_FLOAT, &clamp_min);
    glui->add_edittext_to_panel(clamp_ro, "MAX", GLUI_EDITTEXT_FLOAT, &clamp_max);
    glui->add_checkbox_to_panel(clamp_ro, "Scaling", &scaling_flag, 0, control_cb);

    // Glyphs Parameters
    glui->add_checkbox_to_panel(upper_visu_panel, "Display Glyphs", &draw_glyphs_flag, 0, control_cb);
    GLUI_Panel *glyph_panel = new GLUI_Panel(upper_visu_panel, "");

    GLUI_Listbox *vector_dataset_lb = glui->add_listbox_to_panel(glyph_panel, "Vector Field:", &glyphs.vector_field);
    vector_dataset_lb->add_item(VF_VELOC_ID, "Fluid Velocity Field");
    vector_dataset_lb->add_item(VF_FORCE_ID, "Force Field");

    GLUI_Listbox *scalar_dataset_lb = glui->add_listbox_to_panel(glyph_panel, "Colormap:", &glyphs.scalar_field);
    scalar_dataset_lb->add_item(SF_RHO_ID, "Fluid Density");
    scalar_dataset_lb->add_item(SF_VELOC_ID, "Fluid Velocity Magnitude");
    scalar_dataset_lb->add_item(SF_FORCE_ID, "Force Field Magnitude");
    scalar_dataset_lb->add_item(SF_DIR_ID, "Vector Direction");
    scalar_dataset_lb->add_item(SF_WHITE_ID, "White");

    GLUI_Listbox *glyph_type_lb = glui->add_listbox_to_panel(glyph_panel, "Glypth Type:", &glyphs.glyph_type);
    glyph_type_lb->add_item(GLYPH_ARROW, "Arrow");
    glyph_type_lb->add_item(GLYPH_LINE, "Line");
    glyph_type_lb->add_item(GLYPH_CONE, "Cone");


    GLUI_Panel *hedgehog_panel = new GLUI_Panel (glyph_panel, "Glyph Scaling");
    hh_text = glui->add_statictext_to_panel(hedgehog_panel, "");
    new GLUI_Button(hedgehog_panel, "Increase", HH_INCREASE_ID, control_cb);
    new GLUI_Button(hedgehog_panel, "Decrease", HH_DECREASE_ID, control_cb);

    GLUI_Panel *sample_panel = new GLUI_Panel (glyph_panel, "Number of samples");
    x_sample_text = glui->add_statictext_to_panel(sample_panel, "");
    new GLUI_Button(sample_panel, "Increase", SX_INCREASE_ID, control_cb);
    new GLUI_Button(sample_panel, "Decrease", SX_DECREASE_ID, control_cb);
    glui->add_column_to_panel(sample_panel, false);
    y_sample_text = glui->add_statictext_to_panel(sample_panel, "");
    new GLUI_Button(sample_panel, "Increase", SY_INCREASE_ID, control_cb);
    new GLUI_Button(sample_panel, "Decrease", SY_DECREASE_ID, control_cb);


    new GLUI_Button(glui, "Pause/Play", PP_ID, control_cb);
    new GLUI_Button(glui, "Quit", QT_ID, control_cb);

    update_variables_config_window();

    GLUI_Master.auto_set_viewport();

    glui->set_main_gfx_window(main_window);
}
