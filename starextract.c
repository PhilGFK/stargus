/*
    starextract.c - Extract game data on Maemo
    Copyright (C) 2010  Pali Rohár <pali.rohar@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

#include <gtk/gtk.h>
#include <hildon/hildon.h>

#define TITLE "Stargus"
#define DATA_NEED_COPY "Note: You need the original Starcraft CD\nto extract the game data files.\nData files are needed to run Stargus.\n\nFirst copy Starcraft CD to folder MyDocs/Starcraft\n"
#define DATA_FOUND "Starcraft data files was found in folder MyDocs/Starcraft\n\nPlease be patient, the data may take\na couple of minutes to extract...\n\nPress OK to start extracting data now."
#define DATA_NOT_FOUND "Error: Starcraft data files was not found.\n\nCheck if you have in phone file\nMyDocs/Starcraft/stardat.mpq"
#define EXTRACT_OK "Starcraft data files was successfull extracted."
#define EXTRACT_FAILED "Error: Cannot extract Starcraft data files,\nextract program crashed."

#define EXTRACT_BIN "/opt/stratagus/bin/startool"
#define DATADIR "/home/user/MyDocs/Starcraft"
#define EXTRACTDIR "/opt/stratagus/share/stargus"

#define EXTRACT_COMMAND EXTRACT_BIN DATADIR EXTRACTDIR

inline void message(char * title, char * text) {

	GtkWidget * dialog, * label;
	dialog = gtk_dialog_new_with_buttons(title, NULL, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
	label = gtk_label_new(text);

	gtk_container_add(GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);
	gtk_widget_show_all(GTK_WIDGET (dialog));

	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);

}

int main(int argc, char * argv[]) {

	char buf[1024];
	char version[20];
	char VERSION[20];
	struct stat st;
	FILE * f;

	hildon_gtk_init(&argc, &argv);

	sprintf(buf, "%s/extracted", EXTRACTDIR);
	f = fopen(buf, "r");
	if (f) {
		fgets(version, 20, f);
		fclose(f);
		sprintf(buf, "%s/startool -V", BINPATH);
		f = popen(buf, "r");
		if (f) {
			fgets(VERSION, 20, f);
			pclose(f);
			if (strcmp(version, VERSION) == 0)
				return 0;
		}
	}

	message(TITLE, DATA_NEED_COPY);

	if ( stat("/home/user/MyDocs/Starcraft/stardat.mpq", &st) != 0 ) {

		message(TITLE, DATA_NOT_FOUND);
		return -1;

	}

	message(TITLE, DATA_FOUND);
	int ret = system(EXTRACT_COMMAND);

	if ( ret != 0 ) {

		message(TITLE, EXTRACT_FAILED);
		return -1;

	}

	message(TITLE, EXTRACT_OK);
	return 0;

}
