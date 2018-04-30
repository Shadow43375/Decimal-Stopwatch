#include <pebble.h>

// displace, absolute_displace, and DISPLACE_TEXT are all going to cause problems. 'displace' is used twice. Once for the input to the animation (which is cast from absolute_displace
// making the two redundant...). 'displace' also refers to the CORRECT meaning of clicks*18. DISPLACE_TEXT can potentially stay but only if it is used as multiplier.



Window* window; //check
static Layer* mother_layer; //check
TextLayer *text_layer;  //check
TextLayer *small_text_layer;  //no problem
Layer* time_layer; //check
GRect bounds; // check
int counter = 0; //no problem
int absolute_displacement = 0;
int clicks = 0; //check
char* s_buffer = ""; // no problem
char small_s_buffer[2]; // no problem
static char laps_buffer[1000][10]; // no problem
// char* perm_buffer = ""; 
bool clock_running = false; // no problem
const double mSeconds = 864/10; //no problem
AppTimer *timer; //no problem
ActionBarLayer *action_bar; //no problem
static GBitmap *music_icon_play; //no problem
static GBitmap *music_icon_pause; //no problem
static GBitmap *trash_can; //no problem
static GBitmap *clock_icon; //no problem
static GBitmap *down_arrow_icon; //no problem
static GBitmap *up_arrow_icon; //no problem
bool pressed_before = false; //no problem
bool display_arrows = false; //no problem
// static PropertyAnimation *text_property_animation; // possible ok
static PropertyAnimation *text_property_animation_s_scroll_layer; // possible ok
static PropertyAnimation* text_property_animation_time_layer; // possible ok
char buffer[1000];  // no problem
GRect shrinking_rect; // no problem
GFont font; // check
GSize text_size; // check
static TextLayer* text_layer_array[200]; // check
int displacement; // check
int displacement_2; // check
GRect text_bounds; //check
void click_config_provider_scroll(void *context); // check
void click_config_provider_action(void *context); // check
ScrollLayerCallbacks cbacks; // check
void clear_laps(); // check
static ScrollLayer *s_scroll_layer; //check
int clicks_to_top = 7; // check
int viewport_y = 100; // check
int time_layer_displacement; // NEEDS TO BE ADDED INTO DECIMAL TIME APP!!!
int s_scroll_layer_displacment; // NEEDS TO BE ADDED INTO DECIMAL TIME APP!!!
 


#define ANIMATION_SPEED 25
// #define ACTION_BAR_WIDTH 10
// #define ACTION_BAR_WIDTH PBL_IF_RECT_ELSE(30, 40)
// #define NUM_ACTION_BAR_ITEMS 3


static void anim_started_handler(Animation *animation, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Animation started!");
//   layer_add_child(layer2, text_layer_get_layer(text_layer_array[counter]));
}

static void anim_stopped_handler(Animation *animation, bool finished, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Animation stopped!");
  if(text_layer_get_layer(text_layer_array[clicks])) {
    scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(text_layer_array[clicks]));    
  }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "counter after animation = %d", clicks);
    clicks++;
//     psleep(20000);
}






static void trigger_animation(bool reverse, int delay, int displacement, int displacement_2, bool deleting) {
  
  
    if(clicks<clicks_to_top) {
     // pushes all text up until a certain point is reached
      
    //   GRect from_frame = layer_get_frame(layer);
    //   GRect from_frame = GRect(0,layer1_displacement, bounds.size.w, 100*bounds.size.h);
    //   GRect to_frame = GRect(0, layer1_displacement - 15, bounds.size.w, 100*bounds.size.h);
        GRect from_frame = layer_get_frame(time_layer);
        GRect to_frame = GRect(0,time_layer_displacement - displacement_2, bounds.size.w, 100*(bounds.size.h+3));
        APP_LOG(APP_LOG_LEVEL_DEBUG, "layer1_displacement - displacement_2 = %d", time_layer_displacement - displacement_2);
      
      
      // Create the animation
      text_property_animation_time_layer = property_animation_create_layer_frame(time_layer, &from_frame, &to_frame);
    
      // Configure the animation, these are optional
      animation_set_duration((Animation*) text_property_animation_time_layer, delay); // milliseconds
      animation_set_delay((Animation*) text_property_animation_time_layer, 0); // milliseconds
      animation_set_curve((Animation*) text_property_animation_time_layer, AnimationCurveEaseInOut);
    
      
//         animation_set_handlers((Animation*) text_property_animation_layer, (AnimationHandlers) {
//   .started = anim_started_handler1,
//   .stopped = anim_stopped_handler1
// }, NULL);
    
      // Schedule to occur ASAP with default settings
      animation_schedule((Animation*) text_property_animation_time_layer);    
  }
  
  
  
  if(!deleting) {
    // keeps pushing down old text in order to make room for the newest.
      GRect from_frame_s_scroll_layer;
      GRect to_frame_s_scroll_layer;
      // Set start and end
      if(clicks>0) {
        // the 18 is subtracted to get the displacement of the previous frame. NOT needed if this is the first frame and hence only applies if counter > 0.
    //      from_frame_layer2 = GRect(0, displacement-30,bounds.size.w, 100*bounds.size.h); 
  //        from_frame_layer2 = GRect(3, 100 + displacement ,bounds.size.w, 100*bounds.size.h); 
            from_frame_s_scroll_layer = GRect(3,0 + displacement - 18, bounds.size.w, 100*(bounds.size.h+3)); 
      }
      else if(clicks == 0) {
        // set the location at the apparent origin.
        from_frame_s_scroll_layer = GRect(3,0,bounds.size.w, 100*(bounds.size.h+3));  
      }
  
        to_frame_s_scroll_layer = GRect(3, 0 + displacement, bounds.size.w, 100*(bounds.size.h+3));    
  
   
  //     GRect to_frame_layer2 = GRect(3, 2*displacement + viewport_y, bounds.size.w, 100*bounds.size.h);
    
      // Create the animation
      text_property_animation_s_scroll_layer = property_animation_create_layer_frame((Layer*) s_scroll_layer, &from_frame_s_scroll_layer, &to_frame_s_scroll_layer);
    
      // Configure the animation, these are optional
      animation_set_duration((Animation*) text_property_animation_s_scroll_layer, delay); // milliseconds
      animation_set_delay((Animation*) text_property_animation_s_scroll_layer, delay); // milliseconds
      animation_set_curve((Animation*) text_property_animation_s_scroll_layer, AnimationCurveEaseInOut);
    
    animation_set_handlers((Animation*) text_property_animation_s_scroll_layer, (AnimationHandlers) {
    .started = anim_started_handler,
    .stopped = anim_stopped_handler
  }, NULL);
    
     animation_schedule((Animation*) text_property_animation_s_scroll_layer);      
  }
  
}


void clear_laps() {
  
  
  
for(int i = 0; i < clicks; i++) {
        if(text_layer_array[i]) {
           text_layer_destroy(text_layer_array[i]); 
           APP_LOG(APP_LOG_LEVEL_DEBUG, "i= %d", i);
        }   
}
  clicks = 0;
  displacement = 0;
  displacement_2 = 0;
  GPoint offset_point = GPoint(0,0);
  scroll_layer_set_content_offset(s_scroll_layer, offset_point, true);
  scroll_layer_set_content_size(s_scroll_layer, GSize(144,0));

}



typedef struct currentTime {
    int num_of_hours;
    int num_of_deca_mins;
    int num_of_mins;
    int num_of_deca_secs;
    int num_of_secs;
    int num_of_deci_secs;
} currentTime;

void getDecimalTime(currentTime* current_time)  {
//   current_time->num_of_hours = (counter/100000);
//   current_time->num_of_mins = ((counter-(current_time->num_of_hours*100000))/1000);
//   current_time->num_of_deca_secs = ((counter-((current_time->num_of_hours*100000) + (current_time->num_of_mins*1000)))/100);
//   current_time->num_of_secs = ((counter-((current_time->num_of_hours*100000) + (current_time->num_of_mins*1000) + (current_time->num_of_deca_secs*100)))/10);
//   current_time->num_of_deci_secs = ((counter-((current_time->num_of_hours*100000) + (current_time->num_of_mins*1000) + (current_time->num_of_deca_secs*100) + (current_time->num_of_secs*10)))); 
  
  current_time->num_of_hours = (counter/100000);
  current_time->num_of_deca_mins = ((counter-(current_time->num_of_hours*100000))/10000);
  current_time->num_of_mins = ((counter-((current_time->num_of_hours*100000) + (current_time->num_of_deca_mins*10000)))/1000);
  current_time->num_of_deca_secs = ((counter-((current_time->num_of_hours*100000) + (current_time->num_of_deca_mins*10000) + (current_time->num_of_mins*1000)))/100);
  current_time->num_of_secs = ((counter-((current_time->num_of_hours*100000) + (current_time->num_of_deca_mins*10000) + (current_time->num_of_mins*1000) + (current_time->num_of_deca_secs*100)))/10);
  current_time->num_of_deci_secs = ((counter-((current_time->num_of_hours*100000) + (current_time->num_of_deca_mins*10000) + (current_time->num_of_mins*1000) + (current_time->num_of_deca_secs*100)+(current_time->num_of_secs*10)))); 
}



void display_time() {
  GFont medium_text_font = fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS); 
//   int num_of_hours, num_of_mins, num_of_deca_secs, num_of_secs, num_of_deci_secs;
//   int num_of_deca_mins = 0;
  currentTime currTm;
  currentTime* current_time = &currTm;
  getDecimalTime(current_time);
  
//   sets formatting depending on how much time has elapsed and writes to s_buffer (and small_s_buffer)
  if (counter<1000) {
    snprintf(s_buffer, 10, "%d%d.%d", current_time->num_of_deca_secs, current_time->num_of_secs, current_time->num_of_deci_secs);  
    snprintf(small_s_buffer, 2, "%d", current_time->num_of_deci_secs);
//     text_layer_set_text_color(small_text_layer, GColorClear);
    layer_set_hidden((Layer*) small_text_layer, true);
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  }
  else if (counter >= 1000 && counter < 10000) {
    snprintf(small_s_buffer, 2, "%d", current_time->num_of_deci_secs);
    snprintf(s_buffer, 10, "%d%d:%d%d", current_time->num_of_deca_mins, current_time->num_of_mins, current_time->num_of_deca_secs, current_time->num_of_secs);
    text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(small_text_layer, GTextAlignmentLeft);
//     text_layer_set_text_color(small_text_layer, GColorBlack);
    layer_set_hidden((Layer*) small_text_layer, false);
  }
//   else if (counter >= 10000 && counter < 100000) {
//     snprintf(small_s_buffer, 2, "%d", current_time->num_of_deci_secs);
//     snprintf(s_buffer, 10, "%d:%d%d", current_time->num_of_mins, current_time->num_of_deca_secs, current_time->num_of_secs); 
//     text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
//     text_layer_set_text_alignment(small_text_layer, GTextAlignmentLeft);
// //     text_layer_set_text_color(small_text_layer, GColorBlack);
//     layer_set_hidden((Layer*) small_text_layer, false);
//   }
  else if (counter >= 100000 && current_time->num_of_mins < 10) {
    snprintf(s_buffer, 15, "%d:%d%d:%d%d", current_time->num_of_hours, current_time->num_of_deca_mins, current_time->num_of_mins, current_time->num_of_deca_secs, current_time->num_of_secs); 
    text_layer_set_font(text_layer, medium_text_font);
    text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
//     text_layer_set_text_alignment(small_text_layer, GTextAlignmentRight);
//     text_layer_set_text_color(small_text_layer, GColorClear);
    layer_set_hidden((Layer*) small_text_layer, true);
  }
//   else if (counter >= 100000 && current_time->num_of_mins >= 10) {
//     snprintf(s_buffer, 15, "%d:%d:%d", current_time->num_of_hours, current_time->num_of_mins, current_time->num_of_secs); 
//     text_layer_set_font(text_layer, medium_text_font);
//     text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
// //     text_layer_set_text_alignment(small_text_layer, GTextAlignmentRight);
// //     text_layer_set_text_color(small_text_layer, GColorClear); 
//     layer_set_hidden((Layer*) small_text_layer, false);
//   }
  
  //how to make small_s_buffer not be 50....
  text_layer_set_text(text_layer, s_buffer);
  text_layer_set_text(small_text_layer, small_s_buffer);  
}



void timer_callback(void *data) {	
  
  counter += 1;   
  
  display_time();

	//Register next execution
	timer = app_timer_register(mSeconds, (AppTimerCallback) timer_callback, NULL);
}





void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
    currentTime currTm;
    currentTime* current_time = &currTm;
    getDecimalTime(current_time);

  
  // if the clock is running, then create and add an item to the scroll layer
     if (clock_running) {
       
       
//            if (!display_arrows) {  // check
            scroll_layer_set_content_offset(s_scroll_layer, GPoint(0, 0), true);
//   }
          
         APP_LOG(APP_LOG_LEVEL_DEBUG, "Adding item!");
         
         // calculate the amount the new text layer needs to be displaced by
         displacement = clicks*18; //check
         displacement_2 = clicks*10; //check
       
         // create new text layer and set text
          text_bounds = GRect(0, (-1*time_layer_displacement) + s_scroll_layer_displacment + viewport_y - displacement, 144, 30);
         text_layer_array[clicks] = text_layer_create(text_bounds); // check
         text_layer_set_overflow_mode(text_layer_array[clicks], GTextOverflowModeWordWrap); // check
         text_layer_set_font(text_layer_array[clicks], font); // check
         text_layer_set_background_color(text_layer_array[clicks], GColorClear); // check
         snprintf(laps_buffer[clicks], 10, "%d:%d%d:%d%d:%d", current_time->num_of_hours, current_time->num_of_deca_mins, current_time->num_of_mins, current_time->num_of_deca_secs, current_time->num_of_secs, current_time->num_of_deci_secs); //no problem 
         text_layer_set_text(text_layer_array[clicks], laps_buffer[clicks]); // check

       
      
         // add the newly created text layer to the scroll layer
         scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(text_layer_array[clicks]));   //check
 
       
          // make the magic happen!
          trigger_animation(false, ANIMATION_SPEED, displacement, displacement_2, false); //check
       
       // set the content size dynamically
        if(clicks < clicks_to_top) {    //check
    scroll_layer_set_content_size(s_scroll_layer, GSize(144,0));
  }
  else if(clicks >= 7) {               //check
     scroll_layer_set_content_size(s_scroll_layer, GSize(144, (-1*time_layer_displacement) + s_scroll_layer_displacment + viewport_y + displacement + 95));    
  } 
         
         if(clicks>clicks_to_top) {  //check
           // change the state of the stopwatch to reflect the need to display arrows when stop watch is paused
           display_arrows = true;       // check    
           
//            // make sure that when clicking up the user is always taken to the top of the laps list.
//            scroll_layer_set_content_offset(s_scroll_layer, GPoint(0, 0), true);  //check
         }

     }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "clicks = %d", clicks);
}
 

 
void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
  
       //if the clock is running and the select button is pressed, cancel the timer and change the icon from pause to play and remove clock icon
       if (clock_running) { 
         if(clicks <=8) {
            
           //put the play icon on screen
           action_bar_layer_clear_icon(action_bar,BUTTON_ID_SELECT);
           action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_SELECT, music_icon_play, false);
//            action_bar_layer_set_icon_press_animation(action_bar, BUTTON_ID_SELECT, ActionBarLayerIconPressAnimationNone);
           
           //remove clock icon
           action_bar_layer_clear_icon(action_bar,BUTTON_ID_UP); 
          
          // make sure action bar listeners are in place since there is no need for the scrolling functionality yet.
          action_bar_layer_set_click_config_provider(action_bar, click_config_provider_action);

         }
          // if clicks are greater than six, then need to display scroll arrows and call click_config_provider_scroll to activate scrolling feature.
         else if (clicks > 8) {
              
           //put the play icon on screen
           action_bar_layer_clear_icon(action_bar,BUTTON_ID_SELECT);
           action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_SELECT, music_icon_play, false);
//            action_bar_layer_set_icon_press_animation(action_bar, BUTTON_ID_SELECT, ActionBarLayerIconPressAnimationNone);
           
           //remove clock icon
           action_bar_layer_clear_icon(action_bar,BUTTON_ID_UP); 
           
           //remove the trash icon
           action_bar_layer_clear_icon(action_bar,BUTTON_ID_DOWN);
           
           //put the up and down arrows on the screen
           action_bar_layer_clear_icon(action_bar,BUTTON_ID_UP);
           action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_UP, up_arrow_icon, false);
           action_bar_layer_set_icon_press_animation(action_bar, BUTTON_ID_UP, ActionBarLayerIconPressAnimationNone); 
           action_bar_layer_clear_icon(action_bar,BUTTON_ID_DOWN);
           action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_DOWN, down_arrow_icon, false);
           action_bar_layer_set_icon_press_animation(action_bar, BUTTON_ID_DOWN, ActionBarLayerIconPressAnimationNone);
           
                      
           // set in place scroll functionality
           scroll_layer_set_click_config_onto_window(s_scroll_layer, window);
           cbacks.click_config_provider = &click_config_provider_scroll;
           scroll_layer_set_callbacks(s_scroll_layer, cbacks);
         }
         clock_running = false;
         app_timer_cancel(timer);
 
       }
      
       // if the clock is NOT running and the select button is pressed then create a timer with a call back function to the logic and put icons in place
        else if (!clock_running) {
         clock_running = true;
//          display_arrows = false;
         timer = app_timer_register(mSeconds, (AppTimerCallback) timer_callback, NULL);
          
         //put in place the pause, trash, and clock icons 
         action_bar_layer_clear_icon(action_bar,BUTTON_ID_SELECT);
         action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_SELECT, music_icon_pause, false);
//          action_bar_layer_set_icon_press_animation(action_bar, BUTTON_ID_SELECT, ActionBarLayerIconPressAnimationNone);
         action_bar_layer_clear_icon(action_bar,BUTTON_ID_DOWN);
         action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_DOWN, trash_can, false);
         action_bar_layer_clear_icon(action_bar,BUTTON_ID_UP);
         action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_UP, clock_icon, false);
//          action_bar_layer_set_icon_press_animation(action_bar, BUTTON_ID_UP, ActionBarLayerIconPressAnimationNone);
         pressed_before = true;
         
        // put back in place action bar event listeners
         action_bar_layer_set_click_config_provider(action_bar, click_config_provider_action);
 
       }
  

}


void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
      APP_LOG(APP_LOG_LEVEL_DEBUG, "display_arrows = %d, clock_running = %d", display_arrows, clock_running);
      if(!display_arrows || clock_running) {
         // clear all icons except for the play button
         action_bar_layer_clear_icon(action_bar,BUTTON_ID_UP);
         action_bar_layer_clear_icon(action_bar,BUTTON_ID_SELECT);
         action_bar_layer_clear_icon(action_bar,BUTTON_ID_DOWN);
    
    
         // put in place play button icon
         action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_SELECT, music_icon_play, false);
         action_bar_layer_clear_icon(action_bar,BUTTON_ID_DOWN);
    
    
         counter = 0;
         app_timer_cancel(timer);
         pressed_before = false;
         display_time();

         pressed_before = false;
         pressed_before = false;
         clock_running = false;
         display_arrows = false;
         clear_laps();
         trigger_animation(false, ANIMATION_SPEED, displacement, displacement_2, true);  
      }

}




 void click_config_provider_scroll(void *context)
{
//     window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
//     window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
      

}

 void click_config_provider_action(void *context)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Inside of 'click_config_provider for action function");
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
      

}



// could be useful font used in drawtesting app........
//   font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD); 


/* Load all Window sub-elements */
void window_load(Window *window)
{  
    mother_layer = window_get_root_layer(window);  //check
    font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD); //check
    bounds = layer_get_bounds(mother_layer); //check
    time_layer_displacement = 100*(-bounds.size.h); //check
    s_scroll_layer_displacment = 0; // check
    s_buffer = "00.0"; // possibly ok
  
    time_layer = layer_create(GRect(0,time_layer_displacement, bounds.size.w, 100*(bounds.size.h+3)));
    s_scroll_layer = scroll_layer_create(GRect(3,s_scroll_layer_displacment, bounds.size.w, 100*(bounds.size.h+3)));
  
  
    layer_add_child(mother_layer, time_layer);
    layer_add_child(time_layer, (Layer*) s_scroll_layer);
  

    GFont text_font =  fonts_get_system_font(FONT_KEY_LECO_38_BOLD_NUMBERS); // check
    GFont small_text_font =  fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS); // no problem

  // create text layer for all large text
//        text_layer = text_layer_create(GRect(0,(-time_layer_displacement) + 60,122,38));
      text_layer = text_layer_create(GRect(0,(-time_layer_displacement) + 60,122,45));
      text_layer_set_background_color(text_layer, GColorWhite);
      text_layer_set_text_color(text_layer, GColorBlack);
      text_layer_set_font(text_layer, text_font);
      text_layer_set_text(text_layer, "00.0");
      text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
//       layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  

  
  // create text layer for all small text
      small_text_layer = text_layer_create(GRect(103, (-time_layer_displacement) + 65,122,50));
      text_layer_set_background_color(small_text_layer, GColorWhite);
      text_layer_set_text_color(small_text_layer, GColorBlack);
      text_layer_set_font(small_text_layer, small_text_font);
      text_layer_set_text(small_text_layer, small_s_buffer);
      text_layer_set_text_alignment(small_text_layer, GTextAlignmentLeft);
//       layer_add_child(window_get_root_layer(window), (Layer*) small_text_layer);
  
  
  // create layer for both 'text_layer' and 'small_text_layer'
//       time_layer = layer_create(GRect(0,time_layer_displacement, bounds.size.w, 100*(bounds.size.h+3)));
//       layer_add_child(time_layer, (Layer*) text_layer);
//       layer_add_child(time_layer, (Layer*) small_text_layer);
//       layer_add_child(window_get_root_layer(window), time_layer);
  
  
      display_time();
  
  
      // Initialize the action bar:
      action_bar = action_bar_layer_create();
      // Associate the action bar with the window:
      action_bar_layer_add_to_window(action_bar, window);
//       Set the click config provider:
//       action_bar_layer_set_click_config_provider(action_bar, click_config_provider_action);
  
       // Load the icons
       music_icon_play = gbitmap_create_with_resource(RESOURCE_ID_MUSIC_ICON_PLAY);
       music_icon_pause = gbitmap_create_with_resource(RESOURCE_ID_MUSIC_ICON_PAUSE);
       trash_can = gbitmap_create_with_resource(RESOURCE_ID_TRASH_CAN); 
       clock_icon = gbitmap_create_with_resource(RESOURCE_ID_CLOCK_ICON);
       down_arrow_icon = gbitmap_create_with_resource(RESOURCE_ID_DOWN_ARROW);
       up_arrow_icon = gbitmap_create_with_resource(RESOURCE_ID_UP_ARROW);
  
      // Set the default icons:
       action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_SELECT, music_icon_play, true);
//        action_bar_layer_set_icon_press_animation(action_bar, BUTTON_ID_SELECT, ActionBarLayerIconPressAnimationNone);


  
  
/*  
THE CODE BELOW DEALS WITH THE SCROLL LAYER/LAP FUNCTIONALITY AND IS NOT STABLE YET!
*/  

  
// calculate the displacement that the new text layer will be set to
displacement = clicks*18;
  
// create the information that will be used to generate a new text layer  
text_bounds = GRect(0,displacement,144,30);
  


// Create the ScrollLayer
// s_scroll_layer = scroll_layer_create(GRect(3,s_scroll_layer_displacment, bounds.size.w, 100*(bounds.size.h+3)));
  
  
// create the information that will be used to generate a new text layer  
text_bounds = GRect(0,displacement,144,30);  
  
// hide shadow
 scroll_layer_set_shadow_hidden(s_scroll_layer, true);

// Set the scrolling content size
scroll_layer_set_content_size(s_scroll_layer, GSize(144, clicks*18));

// Let the ScrollLayer receive click events (declaring scroll button listeners is important for SOME reason...)
   scroll_layer_set_click_config_onto_window(s_scroll_layer, window);
   cbacks.click_config_provider = &click_config_provider_scroll;
   scroll_layer_set_callbacks(s_scroll_layer, cbacks);
   window_set_click_config_provider(window, click_config_provider_action);
  
 // Set the click config provider:
   action_bar_layer_set_click_config_provider(action_bar, click_config_provider_action);
  
 
  // Add the TextLayer as a child of the ScrollLayer
// scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(text_layer_array[0]));


  // Add the ScrollLayer as a child of the Window
// layer_add_child(mother_layer, scroll_layer_get_layer(s_scroll_layer));
//   layer_add_child(time_layer, scroll_layer_get_layer(s_scroll_layer));
      layer_add_child(time_layer, (Layer*) text_layer);
      layer_add_child(time_layer, (Layer*) small_text_layer);
}




 
/* Un-load all Window sub-elements */
void window_unload(Window *window)
{
  //Cancel timer
  app_timer_cancel(timer);
 
  //Destroy text layers
  text_layer_destroy(text_layer);
  text_layer_destroy(small_text_layer);
}

 
/* Initialize the main app elements */
void init() {
    window = window_create();
    WindowHandlers handlers = {
        .load = window_load,
        .unload = window_unload
    };
    window_set_window_handlers(window, (WindowHandlers) handlers);
//     window_set_click_config_provider(window, click_config_provider);
    window_stack_push(window, true);
}
 
/* De-initialize the main app elements */
void deinit()
{
    gbitmap_destroy(clock_icon);
    gbitmap_destroy(music_icon_play);
    gbitmap_destroy(music_icon_pause); 
    gbitmap_destroy(trash_can);
    action_bar_layer_destroy(action_bar);
    window_destroy(window);
}
 
/* Main app lifecycle */
int main(void) {
    init();
    app_event_loop();
    deinit();
}