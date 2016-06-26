/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <efl_extension.h>
#include <stdlib.h>
#include <stdio.h>

#include "gles_sample.h"
#include "sample/launcher_interface.h"
#include "user_callbacks.h"

//#include "basic/test_module.h"	// cpp <-> c error
#include "basic/test_forC.h"

static Eina_Bool
naviframe_pop_cb(void* data, Elm_Object_Item* it)
{
	LOGI("naviframe_pop_cb");
	appdata_s* ad = (appdata_s *) data;

	/* Let window go to hide state. */
	elm_win_lower(ad->win);

	return EINA_FALSE;
}

// for music player
Evas_Object *_new_button(void *data, Evas_Object *display, char *name, void *cb)
{
    // Create a button
    Evas_Object *bt = elm_button_add(display);
    elm_object_text_set(bt, name);
    evas_object_smart_callback_add(bt, "clicked", (Evas_Smart_Cb) cb, data);
    elm_grid_pack(display, bt, 80, 10, 20, 10);
    evas_object_show(bt);
    return bt;

}

static void
win_delete_request_cb(void* data, Evas_Object* obj, void* event_info)
{
	ui_app_exit();
}

static void
create_indicator(appdata_s* ad)
{
	elm_win_conformant_set(ad->win, EINA_TRUE);

	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	//elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_TRANSPARENT);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
}

static Evas_Object*
add_win(const char* name)
{
	Evas_Object* win;

	elm_config_accel_preference_set("opengl");
	win = elm_win_util_standard_add(name, "ComputerGraphicsHW3");

	if (!win)
		return NULL;

	if (elm_win_wm_rotation_supported_get(win))
	{
		int rots[4] = {0, 90, 180, 270};
		elm_win_wm_rotation_available_rotations_set(win, rots, 4);
	}

	evas_object_show(win);

	return win;
}


static void
list_selected_cb(void* data, Evas_Object* obj, void* event_info)
{
	LOGI("list_selected_cb\n");
	Elm_Object_Item* it = (Elm_Object_Item *) event_info;
	elm_list_item_selected_set(it, EINA_FALSE);
}

static void
list_item_select_cb(void* data, Evas_Object* obj, void* event_info)
{
	LOGI("list_item_select_cb: list item selected\n");

	appdata_s* ad = (appdata_s *) data;
	Elm_Object_Item* it = (Elm_Object_Item *) event_info;

	// get the name of the item
	ad->cur_sample_name = elm_object_item_text_get(it);

	// get index of the item
	int cnt = 0;
	Elm_Object_Item* iter = it;
	for (; iter != NULL; iter = elm_list_item_prev(iter))
		cnt++;
	LOGI("Sample num: %d\n", cnt);
	ad->cur_sample_num = cnt;

	// create glview for each menu
	Evas_Object* view = get_sample_view(ad);
	elm_naviframe_item_push(ad->navi, ad->cur_sample_name, NULL, NULL, view, NULL);
}

static Evas_Object*
create_main_list(appdata_s* ad)
{
	Evas_Object* list;

	/* List */
	list = elm_list_add(ad->navi);
	elm_list_mode_set(list, ELM_LIST_COMPRESS);
	evas_object_smart_callback_add(list, "selected", list_selected_cb, NULL);

	/* Main Menu Items Here */
	elm_list_item_append(list, "[Term]Teapot", NULL, NULL, list_item_select_cb, ad);
	elm_list_item_append(list, "[Term]City", NULL, NULL, list_item_select_cb, ad);
	elm_list_item_append(list, "[Term]HomeTown", NULL, NULL, list_item_select_cb, ad);
	elm_list_item_append(list, "[Term]color road", NULL, NULL, list_item_select_cb, ad);


	elm_list_go(list);

	return list;
}

// time out callback function
static void
popup_timeout(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	evas_object_del(obj);
	elm_object_text_set(ad->win, "Time out");
}

// clicked popup out
static void
popup_block_clicked(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	evas_object_del(obj);
	elm_object_text_set(ad->win, "Block Clicked");
}


ZoomBtn btns;

// button
void
btn_tab1_cb(void *data, Evas_Object *obj, void *event_info) {

	btns.Btn1 = 1;

/*
	char str[10];
	sprintf(str,"%d",btns.Btn1);

	appdata_s *ad = data;
	ad->popup = elm_popup_add(ad->win);
	elm_popup_align_set(ad->popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	evas_object_size_hint_weight_set(ad->popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(ad->popup, str);

	elm_popup_timeout_set(ad->popup, 0.3);	// 0.3 sec time out
	evas_object_smart_callback_add(ad->popup, "timeout", popup_timeout, ad);


	evas_object_show(ad->popup);
	ad->popupNum = 1;
*/
	//TransZoom::mIsBtn1 = true;
}

void
btn_tab2_cb(void *data, Evas_Object *obj, void *event_info) {

	btns.Btn2 = 1;
/*
	appdata_s *ad = data;
	ad->popup = elm_popup_add(ad->win);
	elm_popup_align_set(ad->popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	evas_object_size_hint_weight_set(ad->popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(ad->popup, "Text popup 2");


	evas_object_smart_callback_add(ad->popup, "block,clicked", popup_block_clicked,	ad);

	evas_object_show(ad->popup);
	ad->popupNum = 2;
*/
	//TransZoom::mIsBtn2 = true;

}

void
btn_tab3_cb(void *data, Evas_Object *obj, void *event_info) {
	btns.Btn3 = 1;
}
void
btn_tab4_cb(void *data, Evas_Object *obj, void *event_info) {
	btns.Btn4 = 1;
}

void
btn_tab5_cb(void *data, Evas_Object *obj, void *event_info) {
	btns.Btn5 = 1;
}

void
btn_tab6_cb(void *data, Evas_Object *obj, void *event_info) {
	btns.Btn6 = 1;
}

static bool
app_create(void* data)
{
    /* Hook to take necessary actions before main event loop starts
     * Initialize UI resources and application's data
     * If this function returns true, the main loop of application starts
     * If this function returns false, the application is terminated. */
	appdata_s* ad = (appdata_s *) data;

	if (!data)
		return false;

	/* Create the window */
	ad->win = add_win(ad->name);

	if (!ad->win)
		return false;

	create_indicator(ad);

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);

	// add conformant
	ad->conform = elm_conformant_add(ad->win);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	// grid for total
	ad->grid_main = elm_grid_add(ad->conform);
	elm_object_content_set(ad->conform, ad->grid_main);
	evas_object_show(ad->grid_main);

	// add navi frame
	//ad->navi = elm_naviframe_add(ad->conform);
	ad->navi = elm_naviframe_add(ad->grid_main);
	eext_object_event_callback_add(ad->navi, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, ad); // back key cb
	evas_object_size_hint_weight_set(ad->navi, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	elm_object_content_set(ad->conform, ad->navi);
	elm_grid_pack(ad->grid_main, ad->navi, 0, 0, 100, 100);
	evas_object_show(ad->navi);

	Evas_Object* main_list = create_main_list(ad);
	Elm_Object_Item* navi_item = elm_naviframe_item_push(ad->navi, "ComputerGraphics Term", NULL, NULL, main_list, NULL);
	elm_naviframe_item_pop_cb_set(navi_item, naviframe_pop_cb, ad);

	evas_object_show(ad->win);

	// initialize buttons
	btns.Btn1 = 0;
	btns.Btn2 = 0;
	btns.Btn3 = 0;
	btns.Btn4 = 0;
	btns.Btn5 = 0;
	btns.Btn6 = 0;

	// button 1

	ad->button1 = elm_button_add(ad->grid_main);
	elm_button_autorepeat_set(ad->button1, EINA_TRUE); // auto enable
	elm_object_text_set(ad->button1, "Left");
	evas_object_smart_callback_add(ad->button1, "repeated", btn_tab1_cb, ad);
	elm_grid_pack(ad->grid_main, ad->button1, 0, 90, 50, 10);
	//evas_object_size_hint_align_set(ad->button, EVAS_HINT_FILL, EVAS_HINT_FILL);
	//evas_object_size_hint_weight_set(ad->button, 0.00001, 0.00001);
	evas_object_show(ad->button1);


	// button 2

	ad->button2 = elm_button_add(ad->grid_main);
	elm_button_autorepeat_set(ad->button2, EINA_TRUE); // auto enable
	elm_object_text_set(ad->button2, "Right");
	evas_object_smart_callback_add(ad->button2, "repeated", btn_tab2_cb, ad);
	elm_grid_pack(ad->grid_main, ad->button2, 50, 90, 50, 10);
	//evas_object_size_hint_align_set(ad->button, EVAS_HINT_FILL, EVAS_HINT_FILL);
	//evas_object_size_hint_weight_set(ad->button, 0.00001, 0.00001);
	evas_object_show(ad->button2);

	// button 3

	ad->button3 = elm_button_add(ad->grid_main);
	elm_button_autorepeat_set(ad->button3, EINA_TRUE); // auto enable
	elm_object_text_set(ad->button3, "Go");
	evas_object_smart_callback_add(ad->button3, "repeated", btn_tab3_cb, ad);
	elm_grid_pack(ad->grid_main, ad->button3, 80, 70, 20, 10);
	//evas_object_size_hint_align_set(ad->button, EVAS_HINT_FILL, EVAS_HINT_FILL);
	//evas_object_size_hint_weight_set(ad->button, 0.00001, 0.00001);
	evas_object_show(ad->button3);

	// button 4

	ad->button4 = elm_button_add(ad->grid_main);
	elm_button_autorepeat_set(ad->button4, EINA_TRUE); // auto enable
	elm_object_text_set(ad->button4, "Back");
	evas_object_smart_callback_add(ad->button4, "repeated", btn_tab4_cb, ad);
	elm_grid_pack(ad->grid_main, ad->button4, 80, 80, 20, 10);
	//evas_object_size_hint_align_set(ad->button, EVAS_HINT_FILL, EVAS_HINT_FILL);
	//evas_object_size_hint_weight_set(ad->button, 0.00001, 0.00001);
	evas_object_show(ad->button4);

	// button up

	ad->button5 = elm_button_add(ad->grid_main);
	elm_button_autorepeat_set(ad->button5, EINA_TRUE); // auto enable
	elm_object_text_set(ad->button5, "▲");
	evas_object_smart_callback_add(ad->button5, "repeated", btn_tab5_cb, ad);
	elm_grid_pack(ad->grid_main, ad->button5, 0, 70, 20, 10);
	//evas_object_size_hint_align_set(ad->button, EVAS_HINT_FILL, EVAS_HINT_FILL);
	//evas_object_size_hint_weight_set(ad->button, 0.00001, 0.00001);
	evas_object_show(ad->button5);

	// button down

	ad->button6 = elm_button_add(ad->grid_main);
	elm_button_autorepeat_set(ad->button6, EINA_TRUE); // auto enable
	elm_object_text_set(ad->button6, "▽");
	evas_object_smart_callback_add(ad->button6, "repeated", btn_tab6_cb, ad);
	elm_grid_pack(ad->grid_main, ad->button6, 0, 80, 20, 10);
	//evas_object_size_hint_align_set(ad->button, EVAS_HINT_FILL, EVAS_HINT_FILL);
	//evas_object_size_hint_weight_set(ad->button, 0.00001, 0.00001);
	evas_object_show(ad->button6);


	// playing music
	/*
    ad->musicButton = elm_button_add(ad->grid_main);
    elm_object_text_set(ad->musicButton, "On");
    evas_object_smart_callback_add(ad->musicButton, "clicked", (Evas_Smart_Cb) cb, data);
    elm_grid_pack(ad->grid_main, ad->musicButton, 80, 10, 20, 10);
    evas_object_show(ad->musicButton);
    */
    create_buttons_in_main_window(ad);



	/* Return true: the main loop will now start running */
	return true;
}


static void
app_control(app_control_h app_control, void* data)
{
	/* Handle the launch request. */
}

static void
app_pause(void* data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void* data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void* data)
{
	/* Release all resources. */
	release_loader();
}

int
main(int argc, char* argv[])
{
	appdata_s ad = {NULL,};
	int ret;

	ui_app_lifecycle_callback_s event_callback = {NULL,};

	ad.name = "tizen_gles_Term";

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE)
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "The application failed to start, and returned %d", ret);
	}

	return ret;
}

