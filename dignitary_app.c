#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>

int VERBOSE	= 1;

char script_name[] = "./gen_data/dignitary_create_script_sqlite.sql";

int can_continue = 1; //boolean value; 0 means application exits
int num_agents = 100; //number of characters to be stored in database
int num_regions = 10;
int num_principalities = 4;
int num_factions = 20;

sqlite3 *db = NULL;

char resource1[] = "Silver";
char resource2[] = "Marble";
char resource3[] = "Iron";
char resource4[] = "Grain";
char resource5[] = "Cattle";

char *resource_list[] = {resource1, resource2, resource3, resource4, resource5};

void parse_cmd(char cmd[]);
void populate_simulation(char *filenamept);
char* choose_random_file_line(char *filename);

int main(int argc, char *argv[]) {	
	srand(time(NULL)); //initialize the rng
	do {
		char cmd[32];
		fputs("dignitary>> ", stdout);
		fgets(cmd, 32, stdin);		
		
		parse_cmd(cmd);

	} while (can_continue);
	return 0;
}

void parse_cmd(char cmd[]) {
	if (strcmp(cmd, "exit\n") == 0) {
		can_continue = 0;
	}
	
	else if (strcmp(cmd, "new simulation\n") == 0) {
		char filename[256];
		char *filenamept = filename;
		char filepath[256];
		char *filepathpt = filepath;
		FILE *create_script;

		fputs("Enter filename: ", stdout);
		fgets(filename, 64, stdin);
		
		filename[strcspn(filename, "\n")] = 0;
		strcat(filename,".db");
		
		sprintf(filepathpt, "./savefiles/%s", filename);
		
		int ret;

		ret = sqlite3_open_v2(filepath, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); //stores return int of sqlite calls - will be 0 if the call succeeded
		
		if (ret) {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		}
		else {
			fprintf(stderr, "Opened database successfully\n");
			char line[256];
			
			create_script = fopen(script_name, "r");
			fseek(create_script, 4L, SEEK_END); //for some reason SEEK_END was found 4 chars late
			int sz = ftell(create_script);
			fseek(create_script, 0L, SEEK_SET);
			
			char script[sz];

			fread(script, sz, 1, create_script);
			
			fclose(create_script);

			ret = sqlite3_exec(db, script, NULL, NULL, NULL);
	
			if (ret) {
				printf("%s\n", script);
				fprintf(stderr, "Database tables not created: %s\n", sqlite3_errmsg(db));
			}
			else {
				fprintf(stderr, "Simulation database tables created successfully\n");
				sqlite3_close(db);
				populate_simulation(filepath);
			}
		}
	}

	else {
		puts("Invalid command.");
	}
}

void populate_simulation(char filepath[]) {
	//first populate the character tables
	fprintf(stderr, "Populating table 'characters'...\n");
	char male_names[] = "./gen_data/male_names.txt";
	char female_names[] = "./gen_data/female_names.txt";
	char surnames[] = "./gen_data/surnames.txt";
	char noun_list[] = "./gen_data/noun_list.txt";
	char adj_list[] = "./gen_data/adjective_list.txt";
	char princip_names[] = "./gen_data/princip_names.txt";
	char region_names[] = "./gen_data/region_names.txt";
	
	char query[256];
	char *sql = query;
	int ret;
	
	FILE *name_list;
	
	int i;
	for (i = 0; i < num_agents; i++) {
		char name[128];
		char *namept = name;
		char f_name[64];
		char l_name[64];
		int gender = (rand() % 100) + 1; //a number between 1 and 100
		if (gender < 50) {
			sprintf(f_name, "%s", choose_random_file_line(male_names));
		}
		else {
			sprintf(f_name, "%s", choose_random_file_line(female_names));
		}

		//now get surname
		sprintf(l_name, "%s", choose_random_file_line(surnames));

		sprintf(namept, "%s %s", f_name, l_name);
		
		//now do the sql query
		sprintf(sql, "INSERT INTO characters (\"id\", \"name\", \"title\") VALUES (%d, \"%s\", \"%s\")", i, name, "n/a");
		
		ret = sqlite3_open_v2(filepath, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
		
		if (!ret)
			ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (ret) {
			fprintf(stderr, "Error populating characters: %s\n", sqlite3_errmsg(db));
		}
	}
	if (!ret) fprintf(stderr, "Table 'characters' succesfully populated\n");
	else fprintf(stderr, "Table 'characters' not populated: %s\n", sqlite3_errmsg(db));
	
	//now populate table 'resource'
	fprintf(stderr,"Populating table 'resource'\n");
	for (i = 0; i < sizeof resource_list / sizeof resource_list[0]; i++) {
		sprintf(sql, "INSERT INTO resource (\"id\", \"name\") VALUES (%d, \"%s\")", i, resource_list[i]);
		ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (ret) fprintf(stderr, "Error populating resource: %s\n", sqlite3_errmsg(db));
	}
	if (!ret) fprintf(stderr, "Table 'resource' successfully populated\n");
	
	//now populate table 'faction'
	fprintf(stderr,"Populating table 'faction'\n");
	for (i = 0; i < num_factions; i++) {
		char faction_name[64];
		char noun[32];
		char adj[32];
		
		sprintf(noun, "%s", choose_random_file_line(noun_list));
		sprintf(adj, "%s", choose_random_file_line(adj_list));
		
		sprintf(faction_name, "The %s %s", adj, noun);
		
		sprintf(sql, "INSERT INTO faction (\"id\", \"name\", \"type\", \"goals\") VALUES (%d, \"%s\", \"%s\", \"%s\")", i, faction_name, "n/a", "n/a");
		ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (ret) fprintf(stderr, "Error populating faction: %s\n", sqlite3_errmsg(db));
	}
	if (!ret) fprintf(stderr, "Table 'faction' successfully populated\n");
	
	//now populate table 'principality'
	fprintf(stderr,"Populating table 'principality\n");
	for (i = 0; i < num_principalities; i++) {
		char name[128];
		int ruling_faction_id;
		
		sprintf(name, "%s", choose_random_file_line(princip_names));
		ruling_faction_id = rand() % num_factions;
		
		sprintf(sql, "INSERT INTO principality (\"id\", \"name\", \"ruling_faction_id\") VALUES (%d, \"%s\", %d)", i, name, ruling_faction_id);
		ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (ret) fprintf (stderr, "Error populating principality: %s\n", sqlite3_errmsg(db));
	}
	if (!ret) fprintf(stderr, "Table 'principality' successfully populated\n");
	
	//now populate table 'region'
	fprintf(stderr,"Populating table 'region'\n");
	for (i = 0; i < num_regions; i++) {
		char name[256];
		int ruling_principality_id;
		
		sprintf(name, "%s", choose_random_file_line(region_names));
		ruling_principality_id = rand() % num_principalities;
		
		sprintf(sql, "INSERT INTO region (\"id\", \"name\", \"ruling_principality_id\") VALUES (%d, \"%s\", %d)", i, name, ruling_principality_id);
		ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (ret) fprintf (stderr, "Error populating principality: %s\n", sqlite3_errmsg(db));

	}
	if (!ret) fprintf(stderr, "Table 'region' successfully populated\n");
	
	sqlite3_close(db);
}

char* choose_random_file_line(char *filename) {
	char line[64];
	char *linept = line;
	int num_lines = 0;
	FILE *file;
	
	file = fopen(filename, "r");
	while (fgets(linept, sizeof(line), file)) { 
		++num_lines;
	}
	fseek(file, 0L, SEEK_SET);
	int rand_line = (rand() % num_lines); //the program will get line line_num + 1
	int j;
	for (j = 0; j < rand_line; j++) {
		fgets(linept, 64, file); //there is probably a more efficient way to iterate through n-1 lines
	}
	fgets(linept, 64, file);
	line[strcspn(line, "\n")] = 0; //remove the trailing newline character
	
	fclose(file);
	
	return linept;
}