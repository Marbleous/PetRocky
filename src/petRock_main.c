#include <pebble.h>

static Window *s_main_window;
static BitmapLayer *s_icon_layer;
static GBitmap *s_icon_bitmap = NULL;
 
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

//static AppSync s_sync;
//static uint8_t s_sync_buffer[64];

static GDrawCommandImage *s_command_image;
static Layer *s_canvas_layer;

/*static const uint32_t ROCK_ICONS[] = {
RESOURCE_ID_ROCK_FRIEND_HAPPY //0
//RESOURCE_ID_ROCK_FRIEND_SAD //1
};*/

/*static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}*/
static void update_proc(Layer *layer, GContext *ctx) {
  // Set the origin offset from the context for drawing the image
  GPoint origin = GPoint(50, 80);

  // Draw the GDrawCommandImage to the GContext
  gdraw_command_image_draw(ctx, s_command_image, origin);
}


static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ROCK_HOME);
  
  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
  s_icon_layer = bitmap_layer_create(GRect(0, 10, bounds.size.w, 80));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));
  
  // Create the canvas Layer
  s_canvas_layer = layer_create(GRect(30, 30, bounds.size.w, bounds.size.h));

  // Set the LayerUpdateProc
  layer_set_update_proc(s_canvas_layer, update_proc);

  // Add to parent Window
  layer_add_child(window_layer, s_canvas_layer);
}
static void window_unload(Window *window) {
  if (s_icon_bitmap) {
    gbitmap_destroy(s_icon_bitmap);
  }

  gbitmap_destroy(s_background_bitmap);
 
  bitmap_layer_destroy(s_background_layer);
  bitmap_layer_destroy(s_icon_layer);
  
  layer_destroy(s_canvas_layer);
  gdraw_command_image_destroy(s_command_image);
}

static void init(void) {
  s_main_window = window_create();
  //window_set_background_color(s_main_window, PBL_IF_COLOR_ELSE(GColorIndigo, GColorBlack));
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_main_window, true);

  // Create the object from resource file
  s_command_image = gdraw_command_image_create_with_resource(RESOURCE_ID_ROCK_FRIEND_HAPPY);

  app_message_open(64, 64);
}

static void deinit(void) {
  window_destroy(s_main_window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}