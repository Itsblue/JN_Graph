
#ifndef __JN_Graph_H__
#define __JN_Graph_H__

#include "SSD1306.h"                          // this is the OLED display library
#ifndef FONT_LIB_V3
#include "fonts.h"                            // fonts library to support the display output
#endif

#define MINDIFF_VAL_MINMAX 10

// define the diagramm axes


#define jn_graph_min(a,b) ((a)<(b)?(a):(b))

//--- this is as preparation of the class creation
class JN_Graph {
  private:

    int stored_vals = 0;                  /** maximum number of real values stored in the given array */
    int stored_vals_first = 0;            /** index of the first real value - sometimes there are other values at the beginning of the array for other reasons */
    int stored_vals_all_info = 0;         /** this is a calculated value - index of first real value + the number of real values */
    int x_axes_start = 0;                 /** x position of the start of the x-axis */
    int x_axes_length = 0;                /** length of the x-axis */
    int y_upper_axes_start = 0;           /** upper y position of the y-axis */
    int y_lower_axes_start = 0;           /** lower y position of the y-axis */
    int meas_point_radius = 1;            /** radius of the meas point as it will be displayed at the graph if the measpoints will not overlap each other*/
		int y_scale = 1;											/** scaling of the y axis - the number means which steps the axis has - like 5 menas 0,5,10, or 3 means 0,3,6 */

    void eval_values(int* upper_val, int* middle_val, int* lower_val, float vals[], int only_act_val, int y_scale);
    void draw_valdiagaxes(SSD1306 display, float val, int upper_val, int mid_val, int lower_val, String title, int only_act_val, String max_scale_x_val, String max_scale_unit);
    void draw_val_curve(SSD1306 display, int upper_val, float lower_val, float vals[], int vals_to_display);

  public:
    JN_Graph ( int stored_vals, int stored_vals_first, int x_axes_start, int x_axes_length, int y_upper_axes_start, int y_lower_axes_start, int meas_point_radius, int y_scale);
    void draw_valdiagscreen(SSD1306 display, float act_val, int only_act_val, float vals[], int vals_to_display, String title, String max_scale_x_val, String max_scale_unit );

};

#endif
