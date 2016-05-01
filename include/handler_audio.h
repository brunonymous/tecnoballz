/** 
 * @file handler_audio.h
 * @brief Handler of the sound and music
 * @created 2004-03-22
 * @date 2012-09-06 
 * @copyright 1991-2014 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/* 
 * copyright (c) 1991-2014 TLK Games all rights reserved
 * $Id$
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
//#define SOUNDISOFF if define, don't compile handler_audio.cc
#ifndef SOUNDISOFF
#ifndef __AUDIOMIXER__
#define __AUDIOMIXER__
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <mikmod.h>
class handler_audio;
#include "tecnoballz.h"

/** Module's positions in a area music */
typedef struct
{
  /** First music of a bricks level */
  Uint32 music_1;
  /** Restart first music */
  Uint32 music_1_loop;
  /** Second music of a bricks level */
  Uint32 music_2;
  /** Restart second music */
  Uint32 music_2_loop;
  /** bricks level completed */
  Uint32 level_completed;
  /** Lost ball in bricks level */
  Uint32 pos_losing;
  /** Shop music */
  Uint32 shop_music;
} musics_pos;

class handler_audio:public virtual tecnoballz
{
private:
    static const Uint32 VOLUME_INC = MIX_MAX_VOLUME / 16;
public:

/* Differents portions in a area music */
typedef enum
{ MUSIC_UNDIVIDED,
  GAME_PORTION,
  WIN_PORTION,
  LOST_PORTION,
  SHOP_PORTION,
  MUSIC_IS_OFF
} MUSIC_PORTIONS;

/** Enumeration of all the musics */
typedef enum
{
  MUSICAREA1,
  MUSICAREA2,
  MUSICAREA3,
  MUSICAREA4,
  MUSICAREA5,
  MUSICGUARD,
  MUSICSCORE,
  MUSICGOVER,
  MUSICINTRO,
  MUSICCONGR,
  TERMIGATOR_MUSIC,
  IN_GAME_MUSIC,
  FRIDGE_IN_SPACE_MUSIC,
  MON_LAPIN_MUSIC
} MUSIC_ENUM;

/** Enumeration of all the sound effects */
typedef enum
{ LOST_LIFE,
  PADDLE_EXPLOSION,
  EXTRA_LIFE,
  /** Collect money or gem */
  COLLECT_MONEY,
  TILT_ALARM,
  /** Collect penalty or bonus capsule */
  COLLECT_CAPSULE,
  /** Vocal "tecnoball" */
  TECNOBALL,
  SHIP_APPEAR,
  /** Paddle transformation */
  PADDLE_TRANSFORMATION,
  GUARDIAN_FIRE,
  PADDLE_FIRE,
  /** Guardian's explosion */
  BIG_EXPLOSION,
  DESTROY_INDESTRUCTIBLE_BRICK,
  SHIP_EXPLODE,
  HIT_GUARDIAN,
  HIT_SHIP,
  HIT_INDESTRUCTIBLE_BRICK1,
  HIT_INDESTRUCTIBLE_BRICK2,
  BALL_HIT_PADDLE,
  BALL_HIT_SIDE,
  BALL_HIT_BRICK1,
  BALL_HIT_BRICK2,
  BALL_HIT_BRICK3,
  BALL_HIT_BRICK4,
  BALL_HIT_BRICK5,
  EJECTOR_OUT,
  ECJECTOR_IN,
  NUM_OF_SOUNDS
} SOUNDS_ENUM;

public:
  /* true if SDL_mixer is enable */
  static bool is_audio_enable;

private:
    handler_audio ();
public:
   ~handler_audio ();
  static handler_audio *get_instance ();
  void stop_music ();
  void play_music (Uint32 music_id);
  void run ();
  void play_level_music (Uint32 area_num, Uint32 level_num);
  void play_shop_music (Uint32 area_num);
  void play_win_music ();
  void play_lost_music ();
  void stop_lost_music ();
  bool is_win_music_finished ();
  Uint32 get_portion_music_played ();
  void play_sound (Uint32 sound_num);
  void disable_sound ();
  void enable_sound ();

private:
  void sound_volume_ctrl ();
  void query_spec ();
  Uint32 area_music (Uint32);
  void initialize ();
  void control_music_position ();
  void play_requested_sounds ();

private:
  static handler_audio *audio_singleton;
  static char sounds_play[NUM_OF_SOUNDS];
  static Mix_Chunk *sound_list[];
  static const musics_pos ptMusicpos[];

private:
  /** Current area number (1 to 5) */
  Uint32 area_number;
  /** Current level number (1 to 12) */
  Uint32 level_number;
  /** Position of the first music of a bricks level
   * in the Protraker module */
  Sint32 music_1_position;
  /** Position of the restart of the current music in
   * the Protraker module */
  Sint32 restart_position;
  /** Position of the second music of a bricks level
   * in the Protraker module */
  Sint32 music_2_position;
  /** Part of the music module currently played: GAME_PORTION,
   * WIN_PORTION, LOST_PORTION or SHOP_PORTION */
  Uint32 current_portion_music;
  /** True if only the music must be played,
   * and not the sound effects, during the phases of intro,
   * shop and game over */
  bool is_only_music;
  /* True if the music is enabled
   * [Ctrl] + [D] or [Ctrl +[S] keys toggle the music on and off */
  bool is_music_enable;
  /** True if sound effect are enabled. [Ctrl] + [F] or
   * [Ctrl] + [S] keys toggle the sound effects on and off */
  bool is_sound_enable;
  /** Current music volume */
  Uint32 music_volume;
  /** Current audio channel volume */
  Uint32 channels_volume;
  /** Identifier of the current music loaded and played */
  Sint32 current_music_id;
  /** Pointer to the Amiga song module (Protracker format) */
  MODULE *song_module;
  /** Size of all waves used for sounds effect */
  Uint32 waves_size;
  /** Current position in the Protraker module */
  Sint32 song_pos;

};
#endif
#endif
