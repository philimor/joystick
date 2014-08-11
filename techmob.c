/***************************************************************************
 *   Copyright (C) 2012 by philippe   *
 *   philippe@localhost.localdomain   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>

#include "common.h"
#include "decode.h"

/* Constantes et MACROS */
#define INPUT_DEVICE "/dev/input/js0"

/* Variables Globales */
int 	input_buf[2]; 		// EndPoint Descriptor, wMaxPacketSize = 8 bytes => 2 x 32bit words
int	input_bufmem[2];	// input buf à t-1
int 	input_file;		// Handler de fichier
char* 	device_path;		// chemin d'accès au Node
int	condexit_thread_read;	// condition de sortie du thread de lecture
classJS oJS; 			// déclaration de l'objet joystick

/* Fonctions */
/*
	thread_read : thread de lecture du device d'entrée
	Retour : 0 si pas d'erreur
*/

void *thread_read ( void * arg )
{
    int i;
    int error_dev;
    int part;

    i = 0;

    while ( condexit_thread_read == 0 ) {
        error_dev = readDevice ( input_file, input_buf, sizeof ( input_buf ), &part );
        i++;
    }
    pthread_exit ( 0 );
}

/*
	thread_process : thread de traitement des données
	Retour : 0 si pas d'erreur
*/
void *thread_process ( void * arg )
{
    int i;
    int condexit ;
    int retval;
    char* buttonpressedmsg;
    bool bChanged;

    condexit=0;
    while ( condexit==0 ) {
        // detect whether a change occured
        bChanged = ( input_buf[0] != input_bufmem[0] ) || ( input_buf[1] != input_bufmem[1] );
        input_bufmem[0] = input_buf[0];
        input_bufmem[1] = input_buf[1];

        // Decoding of the EndPort data: button number and pressed state
        retval = DecodeButton ( input_buf, &oJS );
        if ( oJS.bPressed )
            buttonpressedmsg = " pressed";
        else
            buttonpressedmsg = " released";
        // display of the EndPort data composed of 2 integers
        if ( bChanged ) {
            for ( i=0; i < 2; i++ ) {
                printf ( "%08X ", input_buf[i] );
            }
            printf ( "Button # %i %s", oJS.iButtonPressed,  buttonpressedmsg );
            printf ( "\n" );
        }
        // on sort de la boucle sur appuie sur la touche 10
        if ( input_buf[1] == 0x09010001 ) {
            condexit=1;			// arreter ce thread
            condexit_thread_read = 1;	// arreter le thread de lecture
        }
    }
    pthread_exit ( 0 );
}

/*
openDevice : ouverture du device
file : adresse du handler du device
Node : string, chemin d'accèsau Node (/dev/input/...)
Retour : 0 si pas d'erreur
*/
int openDevice ( int *file, char* Node )
{
    *file = open ( Node, O_RDWR );

    if ( *file < 0 ) {
        perror ( "open" );
        printf ( "erreur %d in opening inode\n", errno );
        return ( errno );
    } else {
        return ( 0 );
    }
}

/*
closeDevice : fermeture du device
file : handler du device
Retour : 0 si pas d'erreur
*/
int closeDevice ( int file )
{
    close ( file );
    return ( 0 );
}

/*
readDevice : lecture du flux EndPoint du device
file 	: handler du device
buf 	: buffer contenant la valeur lue
numtoread: nombre de mots à lire
numread : nombre de mots lus
Retour : 0 si pas d'erreur
*/
int readDevice ( int file, int buf[], int numtoread, int * numread )
{

    * numread = read ( file, buf, numtoread );
    if ( *numread < 0 ) {
        perror ( "Read" );
        printf ( "erreur %d in opening file\n", errno );
        return ( errno );
    } else {
        return ( 0 );
    };
}

int main ( void )
{
    int i;
    int error_dev;
    int part;

    pthread_t th1, th2;
    void *ret;

    printf ( "Test du driver USB TECH MOBILITY \n" );

    error_dev = openDevice ( &input_file, INPUT_DEVICE );

    condexit_thread_read = 0;

    if ( pthread_create ( &th1, NULL, thread_read, "1" ) < 0 ) {
        fprintf ( stderr, "pthread_create error for thread 1\n" );
        exit ( 1 );
    }

    if ( pthread_create ( &th2, NULL, thread_process, "2" ) < 0 ) {
        fprintf ( stderr, "pthread_create error for thread 2\n" );
        exit ( 1 );
    }

    ( void ) pthread_join ( th1, &ret );
    ( void ) pthread_join ( th2, &ret );

    error_dev = closeDevice ( input_file );
    return 0;
}

