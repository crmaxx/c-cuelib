/*
 * File         : cue_common.h
 * Author       : Vincent Cheung
 * Date         : Jul. 31, 2015
 * Description  : Cue library header file, contains common type definations.
 *
 * Copyright (C) 2015, Vincet Cheung<coolingfall@gmail.com>
 */

#ifndef	__CUE_COMMON_H__
#define	__CUE_COMMON_H__

/**
 * Type definition for timecodes formatted as minute:second:frame
 */
typedef struct Timecode {
	int m;
	int s;
	int f;
} Timecode;

/**
 * Type defination for track data.
 */
typedef struct TrackData {
	int track_num;
	char title[1024];
	char performer[1024];
	char date[64];
	char filename[1024];	/* absolute file path */
	Timecode index[100];	/* timecodes */
	int first_index;
	int index_count;
} TrackData;

/**
 * Type defination for cue sheet.
 */
typedef struct CueSheet {
	char title[1024];
	char performer[1024];
	char comment[1024];
	char date[64];
	char genre[512];
	char filename[1024];	/* absolute file path */
	int total_track;

	TrackData tracks_data[128];
} CueSheet;

#endif