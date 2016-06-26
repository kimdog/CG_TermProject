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

#include "user_callbacks.h"

#include <wav_player.h>

#define BUFLEN 256

static const char *wav_file_name = "audio/papa_ost.wav";
static Evas_Object *wav_player_play_bt = NULL;
static int wav_player_id;
static bool wav_playing = false;
static char wav_file[BUFLEN];

static bool wav_repeat = false; // for repeat .

// Callback function called when the WAV file playback is finished.
static void _playback_completed_cb(int id, void *user_data)
{
    if (wav_playing) {
        //PRINT_MSG("Playback finished.");
   		elm_object_text_set(wav_player_play_bt, "On");
   		wav_playing = false;
    }

    if (wav_repeat) {
    	int error_code = wav_player_start(wav_file, SOUND_TYPE_MEDIA, _playback_completed_cb, NULL,
    	                                          &wav_player_id);
    	        if (WAV_PLAYER_ERROR_NONE != error_code) {
    	            dlog_print(DLOG_ERROR, LOG_TAG, "wav_player_start() failed! Error code = %d",
    	                       error_code);
    	            return;
    	        }
    	elm_object_text_set(wav_player_play_bt, "Off");
    	wav_playing = true;

    }
    else {

    	elm_object_text_set(wav_player_play_bt, "On");
    	wav_playing = false;

    }

}

// Callback function invoked when the "Start"/"Stop" button is clicked.
static void __wav_cb_play(appdata_s *ad, Evas_Object *obj, void *event_info)
{
    if (!wav_playing) {
        // Start playing the given WAV file.
        int error_code = wav_player_start(wav_file, SOUND_TYPE_MEDIA, _playback_completed_cb, NULL,
                                          &wav_player_id);
        if (WAV_PLAYER_ERROR_NONE != error_code) {
            dlog_print(DLOG_ERROR, LOG_TAG, "wav_player_start() failed! Error code = %d",
                       error_code);
            return;
        }

        //PRINT_MSG("Playback started.");
        elm_object_text_set(wav_player_play_bt, "Off");
        wav_playing = true;
        // add repeat .
        wav_repeat = true;
    } else {
        // Stop playing the WAV file.

        // repeat Stop .
        wav_repeat = false;

        int error_code = wav_player_stop(wav_player_id);
        if (WAV_PLAYER_ERROR_NONE != error_code) {
            dlog_print(DLOG_ERROR, LOG_TAG, "wav_player_stop() failed! Error code = %d",
                       error_code);
            return;
        }

        //PRINT_MSG("Playback stopped.");
        elm_object_text_set(wav_player_play_bt, "On");
        wav_playing = false;
    }
}

// WAV Player menu creation.
void create_buttons_in_main_window(appdata_s *ad)
{
    // Create the window for the WAV Player.
    //Evas_Object *display = _create_new_cd_display(ad, "WAV Player", NULL);

    // Create the button for the WAV Player.
    //wav_player_play_bt = _new_button(ad, display, "Start", __wav_cb_play);
	wav_player_play_bt = _new_button(ad, ad->grid_main, "On", __wav_cb_play);


    // Get the path to the file which will be played.
    char *shared_path = app_get_resource_path();
    snprintf(wav_file, BUFLEN, "%s%s", shared_path, wav_file_name);
    free(shared_path);
    //PRINT_MSG("File used for playback: %s", wav_file);
}
