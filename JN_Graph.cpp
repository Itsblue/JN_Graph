
#include "JN_Graph.h"


/**------------------------------------------------------------------------------------------------------
* @brief Init the class and set some parameters.
* @author jnoack@itsblue.de
* @param stored_vals        [  in ] maximum number of real values stored in the given array
* @param stored_vals_first  [  in ] index of the first real value - sometimes there are other values at the beginning of the array for other reasons
* @param x_axes_start       [  in ] x position of the start of the x-axis
* @param x_axes_length      [  in ] length of the x-axis
* @param y_upper_axes_start [  in ] upper y position of the y-axis
* @param y_lower_axes_start [  in ] lower y position of the y-axis
* @param meas_point_radius  [  in ] radius of the meas point as it will be displayed at the graph if the measpoints will not overlap each other
* @return void
*/
JN_Graph::JN_Graph ( int stored_vals, int stored_vals_first, int x_axes_start, int x_axes_length, int y_upper_axes_start, int y_lower_axes_start, int meas_point_radius){

  this->stored_vals = stored_vals;
  this->stored_vals_first = stored_vals_first;
  this->stored_vals_all_info = stored_vals_first + stored_vals;
  this->x_axes_start = x_axes_start;
  this->x_axes_length = x_axes_length;
  this->y_upper_axes_start = y_upper_axes_start;
  this->y_lower_axes_start = y_lower_axes_start;
  this->meas_point_radius;

 }

/**------------------------------------------------------------------------------------------------------
* @brief Draw the given array of values to graph a the given display. The code will do scaling and the decoration of the graph by itself.
* @author jnoack@itsblue.de
* @param display          [  in ] object of the display to draw everything on
* @param act_val          [  in ] actual value to be displayed
* @param only_act_val     [  in ] if set to unequal 0 then only the actual value will be update, if set to 0 the full graph is updated
* @param vals             [  in ] array of values to bre displayed
* @param vals_to_display  [  in ] number of values to be displayed - usually used to only show real measured values
* @param title            [  in ] title of the graph
* @param max_scale_x_val  [  in ] max x value of the graph
* @param max_scale_unit   [  in ] unit of the x values of the graph
* @return void
*/
void JN_Graph::draw_valdiagscreen(SSD1306 display, float act_val, int only_act_val, float vals[], int vals_to_display, String title, String max_scale_x_val, String max_scale_unit ){

  int upper_val = 10;
  int mid_val = 5;
  int lower_val = 0;

  // evalute the max, min and mid values out of the lates stored values ...
  eval_values(&upper_val, &mid_val, &lower_val, vals, only_act_val);

  // draw the diagram ...
  draw_valdiagaxes(display, act_val, upper_val, mid_val, lower_val, title, only_act_val, max_scale_x_val, max_scale_unit);

  // draw the curves ...
  draw_val_curve(display, upper_val, lower_val,vals, vals_to_display);

  // show it on the screen
  display.display();
}

/**------------------------------------------------------------------------------------------------------
* @brief  Read the values to be displayed and evaluates the scaling of the y axes, the x-axes is fixed.
*         This is done onyl if there are new values available in the value array.
* @author jnoack@itsblue.de
* @param upper_val        [ out ] max value of the y-axis to display
* @param mid_val          [ out ] mid value of the y-axis to display
* @param lower_val        [ out ] min value of the y-axis to display
* @param vals             [  in ] array of values
* @param only_act_val     [  in ] if set to unequal 0 then only the actual value will be update, if set to 0 the full graph is updated
* @return void
*/
void JN_Graph::eval_values(int* upper_val, int* middle_val, int* lower_val, float vals[], int only_act_val)
{
  float max_val = -100;
  float min_val = 100;
  //float mid_val = 0;
  //int diff_vals = 0;
  float act_val = 0;

  // check if there are new values ...
  if(only_act_val == 0)
  {
    //loop over the full array and min and max
    // multiple of 5 is the goal ...
    for(int nr = stored_vals_first;nr < (stored_vals + stored_vals_first) ; nr++){
      act_val = vals[nr];
      min_val = (nr==0 || act_val < min_val) ? act_val:min_val;
      max_val = (nr==0 || act_val > max_val) ? act_val:max_val;
    }
    *upper_val = ((int)((max_val/5)+1.5))*5;
    *lower_val = ((int)((min_val/5)-0.5))*5;
    *middle_val = (int)((*upper_val - *lower_val)/2) + *lower_val;
  }
}

/**------------------------------------------------------------------------------------------------------
* @brief Draw the diagram axes, scales, actual value and diagram title
* @author jnoack@itsblue.de
* @param display          [  in ] object of the display to draw everything on
* @param val              [  in ] actual value to display
* @param upper_val        [  in ] max value of the y-axis to display
* @param mid_val          [  in ] mid value of the y-axis to display
* @param lower_val        [  in ] min value of the y-axis to display
* @param title            [  in ] title of the graph
* @param only_act_val     [  in ] if set to unequal 0 then only the actual value will be update, if set to 0 the full graph is updated
* @param max_scale_x_val  [  in ] max x value of the graph
* @param max_scale_unit   [  in ] unit of the x values of the graph
* @return void
*/
void JN_Graph::draw_valdiagaxes(SSD1306 display, float val, int upper_val, int mid_val, int lower_val, String title, int only_act_val, String max_scale_x_val, String max_scale_unit){
  String val_c = title + ": " + String((int)val) + "°C";

  // show the title
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);

  if(only_act_val != 0){
    // only write the actual temeperature value ...
    display.setColor(BLACK);
    display.fillRect(x_axes_start, 0 ,x_axes_length , y_upper_axes_start-1 );
    display.setColor(WHITE);
    display.drawString(x_axes_start+(x_axes_length)/2 , 6 , val_c);
  }
  else
  {
    // redraw everything ...
    display.clear();
    display.drawString(x_axes_start+(x_axes_length)/2 , 6 , val_c);

    // draw the axis - all are dotted, the middle one is more dotted :-)
    for(int x = x_axes_start;x<=(x_axes_start + x_axes_length);x=x+3){
      // upper axis ...
      display.setPixel(x, y_upper_axes_start);
      // lower axis ...
      if(x <= (x_axes_start + x_axes_length)){
        display.setPixel(x, y_lower_axes_start);
      }
      // middle axis ...
      //if(x%2 == 0 ){
      //  display.setPixel(x, y_upper_axes_start +(y_lower_axes_start - y_upper_axes_start)/2);
      //}
    }

    // display the legend (upper, middle and lower values at the left side
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(Dialog_plain_8);
    // the upper y value ...
    display.drawString(x_axes_start - 6, y_upper_axes_start -5 , String(upper_val));
    // the middle value ...
    display.drawString(x_axes_start - 6, y_upper_axes_start +(y_lower_axes_start - y_upper_axes_start)/2 - 5 , String(mid_val));
    // the lower y value ...
    display.drawString(x_axes_start - 6, y_lower_axes_start -5, String(lower_val));
    // the left x value ...
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(x_axes_start , y_lower_axes_start+2  , "-" + max_scale_x_val);
    // the middle x value ...
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(x_axes_start+(x_axes_length)/2, y_lower_axes_start+2  , max_scale_unit);
    // the right x value ...
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(x_axes_start+x_axes_length, y_lower_axes_start+2  ,"now");

  }

  // calculate the sizeof the actual value string and remove the diagram at this postion before
  //display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.setFont(ArialMT_Plain_24);
  //String val_c = String(val) + "°C";
  //int16_t string_width = display.getStringWidth(val_c ) + 2;
  //display.setColor(BLACK);
  //display.fillRect(126-string_width, 38,string_width , 26);
  // display the actual value - remove the diagram at this postion before
  //display.setColor(WHITE);
  //display.drawString(127, 39, String(val) + "°C");
}

/**------------------------------------------------------------------------------------------------------
* @brief Draw the diagram curve
* @author jnoack@itsblue.de
* @param display          [  in ] object of the display to draw everything on
* @param upper_val        [  in ] value diplayed at the upper end of the y axis
* @param lower_val        [  in ] value diplayed at the lower end of the y axis
* @param vals             [  in ] array of values to bre displayed
* @param vals_to_display  [  in ] number of values to be displayed - usually used to only show real measured values
* @return void
*/
void JN_Graph::draw_val_curve(SSD1306 display, int upper_val, float lower_val, float vals[], int vals_to_display)
{
  float x_step_width = (x_axes_length / jn_graph_min(stored_vals,x_axes_length));
  float y_step_width = (y_lower_axes_start - y_upper_axes_start)/(upper_val - lower_val);
  float x_val = 0;
  float y_val = 0;
  float x_val_last = 0;
  float y_val_last = 0;
  float x_zero = (x_axes_start + x_axes_length);// - int(x_step_width/2));

  // we only have specific number of points at the display the we can use to show values ... even if there are more stored in the array
  for(int nr = stored_vals_first ; nr<( jn_graph_min((stored_vals + stored_vals_first),(x_axes_length+stored_vals_first))-1); nr++){
    if(vals_to_display > (nr - stored_vals_first))
    {
      //only draw values that we have been really measured
      x_val = x_zero - (x_step_width * (nr - stored_vals_first) );
      y_val = (y_step_width * (upper_val - vals[nr])) + y_upper_axes_start;
      if(meas_point_radius > 1 && x_axes_length/jn_graph_min(stored_vals,x_axes_length) > (meas_point_radius + 2)){
        display.fillCircle(int(x_val), int(y_val), meas_point_radius);
      }
      if(nr > stored_vals_first){
       display.drawLine(int(x_val), int(y_val), int(x_val_last), int(y_val_last));
      }
      x_val_last = x_val;
      y_val_last = y_val;
    }
  }
}
