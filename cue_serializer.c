/*
 * File         : cue_serializer.c
 * Author       : Vincent Cheung
 * Date         : Jul. 31, 2015
 * Description  : Cue file serializer library.
 *
 * Copyright (C) 2015, Vincet Cheung<coolingfall@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>  
#include <unistd.h>
#include "cue_utils.h"
#include "cue_serializer.h"


/**
 * Serialize cue sheet into a file.
 */
void serialize_cue(CueSheet *cue_sheet, const char *out_path)
{
	int i;

	if (!out_path || !cue_sheet || cue_sheet->total_track <= 0)
	{
		return;
	}

	for (i = 0; i < cue_sheet->total_track; i ++)
	{
		if (!strlen(cue_sheet->tracks_data[i].title))
		{
			printf("track title cannot be null");
			return;
		}
	}

	/* clear file if existed */
	#if defined(UNIX) && !defined(WIN32)
	truncate(out_path, 0);
	#endif

	FILE *fp = fopen(out_path, "w");
	if (!fp)
	{
		return;
	}

	/* clear file if existed */
	#if defined(WIN32) && !defined(UNIX)
	_chsize(fileno(fp), 0);
	#endif

	/* write global data, such as genre, album performer, file and so on */
	if (strlen(cue_sheet->genre))
	{
		write_string("REM GENRE ", fp);
		write_string(cue_sheet->genre, fp);
		write_string("\n", fp);
	}

	if (strlen(cue_sheet->date))
	{
		write_string("REM DATE ", fp);
		write_string(cue_sheet->date, fp);
		write_string("\n", fp);
	}

	if (strlen(cue_sheet->comment))
	{
		write_string("REM COMMENT ", fp);
		write_string(cue_sheet->comment, fp);
		write_string("\n", fp);
	}

	if (strlen(cue_sheet->performer))
	{
		write_string("PERFORMER \"", fp);
		write_string(cue_sheet->performer, fp);
		write_string("\"\n", fp);
	}

	if (strlen(cue_sheet->title))
	{
		write_string("TITLE \"", fp);
		write_string(cue_sheet->title, fp);
		write_string("\"\n", fp);
	}

	int len = strlen(cue_sheet->filename);
	char relative_path[len];
	int index = last_index_of(cue_sheet->filename, "/");
	if (index > 0)
	{
		substring(relative_path, cue_sheet->filename, index + 1, strlen(cue_sheet->filename));
	}
	else
	{
		strcpy(relative_path, cue_sheet->filename);
	}

	write_string("FILE \"", fp);
	write_string(relative_path, fp);
	write_string("\" WAVE\n", fp);

	/* write track data */
	for (i = 0; i < cue_sheet->total_track; i ++)
	{
		TrackData track_data = cue_sheet->tracks_data[i];

		/* write track audio */
		int track_num = track_data.track_num;
		char num_string[4] = {0};
		
		if (track_num < 10)
		{
			sprintf(num_string, "0%d", track_num);
		}
		else
		{
			sprintf(num_string, "%d", track_num);
		}

		write_string("  TRACK ", fp);
		write_string(num_string, fp);
		write_string(" AUDIO\n", fp);

		/* write title */
		write_string("    TITLE \"", fp);
		write_string(track_data.title, fp);
		write_string("\"\n", fp);

		/* write track performer */
		if (strlen(track_data.performer))
		{
			trim(track_data.performer);
			write_string("    PERFORMER \"", fp);
			write_string(track_data.performer, fp);
			write_string("\"\n", fp);
		}

		if (track_data.index_count == 0)
		{
			write_string("    INDEX 01 00:00:00\n", fp);
		}
		else
		{
			for (int i = track_data.first_index; i <= track_data.index_count; i++) {
				fprintf(fp, "    INDEX %02d %02d:%02d:%02d\n", i, track_data.index[i].m, track_data.index[i].s, track_data.index[i].f);
			}
		}
	}

	fclose(fp);
}