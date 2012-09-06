/** 
 * @file controller_bricks.h
 * @brief Control the bricks in bricks levels
 * @created 1996-11-13
 * @date 2012-09-06 
 * @copyright 1991-2012 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/* 
 * copyright (c) 1991-2012 TLK Games all rights reserved
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
#ifndef __CONTROLLER_BRICKS__
#define __CONTROLLER_BRICKS__
class controller_bricks;
class sprite_paddle;
#include "../include/tecnoballz.h"
#include "../include/sprite_brick.h"

/**
 * Structure of one simple bricks into the map of current level
 */
typedef struct
{
  /** offset on the pixels brick from the bitmap source */
  Sint32 source_offset;
  /** Brick is currently display */
  bool is_displayed;
  /** Brick counter */
  Sint32 number;
  /** Offscreen offset for drawing brick */
  Sint32 pixel_offset;
  /** Sprite used to display brick if bob_ground = true */
  sprite_brick *sprite;
  /** Brick horizontal position in the bricks bitmap
   * 0, 2, 4, 6, 8, 10 or 12 */
  Sint32 h_pos;
  /** Brick vertical position in the bricks bitmap
   * 0, 1, 2, 3, 4, 5, 6, 7, or 8 */
  Sint32 v_pos;
  /** Pointer to the address of the buffer containing a save of
   * the background which is under a brick (if has_backfground = true)
   * modulo = 0 */
  Sint32 *save_background;
  /** Brick color, 17 colors possibles */
  Uint32 color;
}
brick_info;

/**
 * Structure for draw brick or restore the background
 */
typedef struct
{
  /** If true if indestructible power 1 brick */
  bool is_indestructible;
  /** If true brick was destroyed by the Gigablitz */
  bool is_gigablitz_destroyed;
  /** X-coordintate of the collision */
  Sint32 xcoord_collision;
  /** X-coordintate of the collision */
  Sint32 ycoord_collision;
  /** The last paddle touched by the ball or the paddle was fired */
  sprite_paddle *paddle;
  /** Brick number touched */
  Sint32 number;
  /** If true restore backgound, redraw brick otherwise */
  bool is_background;
  /** Offscreen offset for drawing brick */
  Sint32 pixel_offset;
  /** Sprite used to display brick if bob_ground = true */
  sprite_brick *sprite;
  /** Pointer to the brick in the map */
  brick_info *brick_map;
}
brick_redraw;

#include "../include/sprite_paddle.h"
#include "../include/bitmap_data.h"
#include "../include/right_panel_score.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/sprite_ship.h"

class controller_bricks:public objects_list < sprite_object, controller_bricks >
{
  friend class controller_balls;
  friend class sprite_projectile;
  friend class controller_gigablitz;
  friend class sprite_ship;

public:

  /** Maximum number of bricks to erase */
  static const Uint32 MAXBRIKCLR = 2 << 8;
  /** Numbers of bricks peer line */
  static const Uint32 MAX_OF_BRICKS_HORIZONTALLY = 16;
  /** Numbers of lines of bricks */
  static const Uint32 MAX_OF_BRICKS_VERTICALLY = 30;
  /** Maximum number of bricks */
  static const Uint32 MAX_OF_BRICKS = MAX_OF_BRICKS_HORIZONTALLY
    * (MAX_OF_BRICKS_VERTICALLY + 8);

private:
  /** Brick width in pixels in low-res */
  static const Uint32 BRICK_WIDTH = 16;
  /** Brick height in pixels in low-res */
  static const Uint32 BRICK_HEIGHT = 7;
  static const Sint32 offBri_DD = 1;
  static const Sint32 offBri_GG = -1;
  static const Sint32 offBri_HH = -MAX_OF_BRICKS_HORIZONTALLY;
  static const Sint32 offBri_BB = MAX_OF_BRICKS_HORIZONTALLY;
  static const Sint32 offBri_BG = MAX_OF_BRICKS_HORIZONTALLY - 1;
  static const Sint32 offBri_BD = MAX_OF_BRICKS_HORIZONTALLY + 1;
  static const Sint32 offBri_HG = -MAX_OF_BRICKS_HORIZONTALLY - 1;
  static const Sint32 offBri_HD = -MAX_OF_BRICKS_HORIZONTALLY + 1;
  /** Number of bricks per row in a level */ 
  static const Uint32 BRICKS_MAP_WIDTH = 10;
  /** Number of bricks per column in a level */ 
  static const Uint32 BRICKS_MAP_HEIGHT = 17;
  static const Uint32 LEVEL_SIZE = BRICKS_MAP_WIDTH * BRICKS_MAP_HEIGHT;
  /** Number of levels in a area */
  static const Uint32 MAX_OF_LEVELS_IN_AREA = 10;
  static const Uint32 SIZEOFAREA = MAX_OF_LEVELS_IN_AREA * LEVEL_SIZE;
  static const Uint32 BRKYOFFSET = 8;   //y-offset between 2 bricks 

private:
  /** Bitmap of the set of current bricks */
  surface_sdl *bitmap_bricks;
  /** Number of bricks in the current level */
  Uint32 num_of_bricks;
  /** Less bricks counter */
  Sint32 less_bricks_count;
  /** Time delay for the "less bricks" option */
  Sint32 less_bricks_delay;
  /** Map of the bricks currently displayed on the screen, 30 lines
   * of 16 breack each */
  brick_info *bricks_map;
  /** Width in pixels of a set of bricks */
  Uint32 bricks_height;
  /** Height in pixels of a set of bricks */
  Uint32 bricks_width;
  /** Brick's width in pixels */
  Uint32 brick_width;
  /** Brick's height in pixels */
  Uint32 brick_height;
  /** Brick's size in bytes */
  Uint32 brick_size;
  /** Pixels offset on the first indestructible brick */
  Sint32 indestructible_offset;
  Sint32 brkyoffset;            //y-offset between 2 bricks 
  Sint32 shadow_offset;         //size of shadow in pixels (3 or 6)
  Sint32 shadow_left;            //(4 or 8)
  Sint32 shadow_yoff;            //space between 2 bricks (1 or 2)
  Sint32 shadow_top1;            //(2 or 4)
  brick_redraw * bricks_redraw;    // table de reaffichage 
  Sint32 briqueSave;            // pointeur sur "bricks_redraw"

  /** If true sprites were added to sprites list */
  bool were_sprites_added;
  bool is_cycling;
  Uint32 cycling_count;

protected:
  Sint32 brique_clr;            // pointeur sur "bricks_redraw"
  /** Buffer used to save the background under bricks
   * if has_background = true  */
  char *background_under_bricks;
  Sint32 offsSource;            // adresse de la page brique
  Sint32 offsDestin;            //
  Sint32 *adr_source;           // adresse de la page brique
  Sint32 *adr_desti1;           // adresse du buffer (ecran travail)
  Sint32 *adr_desti2;           // adresse du tampon (ecran restitution) 

public:
    controller_bricks ();
   ~controller_bricks ();
  void first_init ();
  void initialize ();
  void add_to_sprites_list ();
  void start_cycling();
  void color_cycling();
  bool update ();
  Uint32 get_num_of_bricks ();
  void less_bricks ();
  void draw_brick (char *pixels, Sint32 offset, Sint32 color);
  void clr_bricks ();
  brick_info* get_bricks_map ();
  brick_info* get_bricks_map (Sint32 xcoord, Sint32 ycoord);
  brick_redraw* get_bricks_redraw (); 
  brick_redraw* get_bricks_redraw_next (); 
  void bricks_redraw_next (); 


  Sint32 get_brick_width ();
  Sint32 get_indestructible_offset ();
  Sint32 getYOffset ();

private:
  void load_level (Sint32 area_nu, Sint32 level_nu);
  void draw_bricks_shadows ();
  void draw_bricks ();
  void set_bricks_palette ();
  void save_background ();
};
#endif




/*

MAX_OF_BRICKS = 480 briques a l'ecran


 bricks_map 
  fonction : collision des balles/tirs avec les 480 briques de l'ecran
  taille   : 7*480 = 3360 octets
  type     : type structure brick_info 6 mots et 1 pointeur
   offset : adresse relative du bitmap de la brique, sert pour les collisions/resistance de la brique controller_balls::vitusBrick()
   is_displayed : adresse relative du bitmap de la brique
   number : numero de la brique (sert a rien ???)
   pixel_offset : adresse ecran relative d'affichage (dans le buffer et le tampon)
   h_pos : abscisse de la brique dans la page Gfx brique (0,1,2,3,4,5,6,7 ou 8)
   v_pos : ordonnee de la brique dans la page Gfx brique (0,2,4,6,8,10 ou 12)
   save_background : adresse absolue du fond 4 couleurs sous la brique (modulo 0)



 -------------------------------------------------------------------------------
 fichier briques : "zebricks.bmp" 1220 x 254 pixels
 -------------------------------------------------------------------------------
 Ce fichier contient 10 jeux de briques (un jeu de
 brique est choisis au hasard a chaque tableau)
 
 Un jeu de brique :
 Taille : 224x126 pixels : 63 briques
 Le fichier brique contient 7 colonnes de 9 lignes briques de 32x14 pixels 
 Les 8 premieres lignes sont les briques "normales". Chaque ligne est une brique
 avec 7 niveaux de resistance. La colonne 7 represente la plus forte resistance,
 et la colonne 1 la resistance la plus faible. La colonne 0 est vide, la
 brique est detruite.
 La 9ieme ligne contient les briques indestrutibles.


 -------------------------------------------------------------------------------
 fichier tableau : "tableau.data" : 34000 octets
 -------------------------------------------------------------------------------
 Ce fichier contient 100 tableaux. Le jeu comporte 50 tableaux, repartis en
 5 zones de 10 niveaux. Pour un niveau il existe deux tableaux possibles choisi
 au hasard. Les 100 tableaux se suivent sequentiellement dans le fichier :
 les 2 tableaux du niveau 1 - zone 1, les 2 tableaux du niveau 2 - zone 1, ...
 
 Un tableau (340 octets) :
 Se compose de 10 colonnes de 17 lignes. Une brique est represente par un mot 
 de 16 bits.
 Les bits 0 a 7 contiennent la position x sur la page brique de 0 a 12 (de 2 en 2)
 Les bits 8 a 15 contiennent la position y sur la page brique de 0 a  8 (de 1 en 1) 
 Si x et y sont a 0, c'est qu'il n'existe pas de brique.
 
 Le fichier est lu au debut de chaque niveau et les valeurs sont recopiees dans
 la "bricks_map" tableau de structure "brick_info"
 
 
 -------------------------------------------------------------------------------
 tableau "bricks_map" : (tableau de structures "brick_info")
 -------------------------------------------------------------------------------
 Ce tableau est un tableau de type "brick_info". Chaque entree du tableau
 represente une brique a l'ecran. Le tableau represente 16 colonnes, de
 30 lignes de briques, et couvre donc la totalite de l'ecran de jeu, bien que
 seulement 10 colonnes de 17 lignes soient utilisees, ce qui correspond a la
 taille d'un tableau lu dans le fichier "tableau.data". 
 
 La structure "brick_info", contient des informations sur une brique a l'ecran
 utilisee pour les collisions avec les balles, les tirs et le gigablitz.
 
 
  offset => adresse source relative de la brique (resistance)
  is_displayed => adresse source relative de la brique 
  number => numero de la brique par rapport a la premiere
  pixel_offset => adresse ecran relative d'affichage (buffer et tampon)
  h_pos => abscisse de la brique dans la page graphique brique 0, 2, 4, 6, 8, 10 ou 12  
  v_pos => ordonnee de la brique dans la page graphique brique 0, 1, 2, 3, 4, 5, 6, 7, ou 8 
 *save_background => adresse du fond de la brique pour l'effacer (modulo 0)


Jeu original :
3 tables de 4 octets par brique


set_table  =>
set_table2 => copie de set_table (effacement)
set_table3 => tableau actuel valeur (collision)

set_table  + 0 = numero brique
set_table  + 2 = adresse relative de la brique dans le fichier graphique brique *collision*
set_table3 + 0 = position x de la brique dans le fichier graphique brique (+JMM) *decremente resitance*
set_table3 + 2 = position y de la brique dans le fichier graphique brique (+JMM)

BRI.X		(2 octets)		Position X de la balle (-1 = brique indes. detruitre / 512 = detruit au gigablitz)
BRI.Y		(2 octets)		Position Y de la balle
BRI.RAK		(4 octets)		Adresse de la structure raquette
BRI.NUM		(2 octets)		Numéro brique
BRI.FLAG	(2 octets)		0=Affiche brique / 1=Affiche décor
BRI.ADR		(2 octets)		Adresse relative d'affichage


RIZ		=BK.X*4		Largeur en octets ligne map-editor

 -------------------------------------------------------------------------------

 -------------------------------------------------------------------------------
typedef struct
{
  Sint32                    xcoord_collision;                          // abcsisse ecran de la balle
  Sint32                    ycoord_collision;                          // ordonnee ecran de la balle
  sprite_paddle             *raquettePT;                          // pointeur sur la raquette qui a touche cette balle en dernier
  Sint32                    number;                          // numero de la brique touchee
  Sint32                    is_background;                          // 1=affiche le decor du fond ou 0=affiche la brique
  Sint32                    pixel_offset;                          // offset d'affichage ecran de la brique
  brick_info             *brick_map;                          // adresse de la brique dans "bricks_map"

*/
