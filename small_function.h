#define _CRT_SECURE_NO_WARNINGS
#ifndef _SMALL_FUNCTION_H_
#define _SMALL_FUNCTION_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#define BUFFER 40
#define LAYOUT 50
#define SYSFILE "system_capacity.txt"
#define MIN_POINTS 1
#define MAX_POINTS 15
#define BUFFER_2 256

//structure
typedef struct {
	char name[50];
	char post[3];
	int code;
}detail;

typedef struct {
	detail user;
	char pass[50];
}system_users;

typedef struct {
	detail event;
	int point;
	detail prs_in_chrg;
	struct {
		int day;
		int month;
		int year;
	}date[2];
	char type[30];
	char* category[30];
}activities;

typedef struct {
	detail stdt_data;
	int point;
	int module_count;
	int event_count;
	detail* module_dn;
	detail* evnt_dn;
}point_mngmt;

typedef struct {
	detail module;
	char major[40/*BUFFER*/];
	int point;
}module;

typedef struct {
	int std_cd;
	char major[30];
	char* category[30];
}std_dtl;

struct users {
	int input, userid[6], col;
	char input_lvl[10], lvl[10], name[100], pass[100], pass2[100], file[200], inputID[100], ID[100], Tuserid[100];
};

struct EDusers
{
	char fname[100];
	char ftype[3];
	int fID;
	char fpassword[100];
};

struct users_2
{
	char name[50];
	char type[3];
	int ID;
	char password[100];
};

typedef struct {
	char message[100];
	detail sender;
	detail receiver;
}request;

//function & validation
int ch_valid(char ch_to_chck[3], int start, int end) {
	int lgth = strlen(ch_to_chck);
	char* chptr = ch_to_chck;
	for (int lp = 0; lp < lgth; lp++) {
		if (!(*(chptr + lp) >= 48 && *(chptr + lp) <= 57)) {
			printf("\n-------- Error: Invalid Character --------\n\n");
			return -1;
		}
	};
	int ch = atoi(ch_to_chck);
	if (ch >= start && ch <= end) {
		return 0;
	}
	else {
		printf("\n-------- Error: Choice is out of range (%d - %d) --------\n\n", start, end);
		return -1;
	}
}

void tablelise(char* title, char opt[][40], int num_elem) {
	int lgst_lght = 40;
	for (int elem = 0; elem < num_elem; elem++) {
		if (strlen(opt[elem]) > lgst_lght) {
			lgst_lght = strlen(opt[elem]) + 10;
		}
	}

	printf("\033[4m%*c\033[0m\n", lgst_lght - 1, '_');

	if (strlen(title) % 2 == 0) {
		printf("\033[4m%-*s%-*s|\033[0m\n", (((lgst_lght - strlen(title)) / 2) - 2), "|", (((lgst_lght - strlen(title)) / 2) + strlen(title)), title);
	}
	else {
		printf("\033[4m%-*s%-*s|\033[0m\n", (((lgst_lght - strlen(title)) / 2) - 1), "|", (((lgst_lght - strlen(title)) / 2) + strlen(title)), title);
	}
	printf("|%*s%*s\n", 3, "|", (lgst_lght - 5), "|");
	for (int elem = 0; elem < num_elem; elem++) {
		if (strlen(opt[elem]) % 2 == 0) {
			printf("|%d.%-*s%-*s|\n", (elem + 1), (((lgst_lght - 4) - strlen(opt[elem])) / 2), "|", (((lgst_lght - 4) - strlen(opt[elem])) / 2) + strlen(opt[elem]) - 1, opt[elem]);
		}
		else {
			printf("|%d.%-*s%-*s|\n", (elem + 1), (((lgst_lght - 4) - strlen(opt[elem])) / 2), "|", (((lgst_lght - 4) - strlen(opt[elem])) / 2) + strlen(opt[elem]), opt[elem]);
		}
	}
	printf("\033[4m|%*s%*s\033[0m\n\n\n", 3, "|", (lgst_lght - 5), "|");
}

int choice_validation(int start, int end) {

	char choice_user[3];
	int choice, returned;

	do {
		printf("Choice: ");
		scanf(" %s", choice_user);
		returned = ch_valid(choice_user, start, end);
	} while (returned == -1);
	choice = atoi(choice_user);

	return choice;
}

void read_user_data(char file_name[50], int* count, system_users** user) {

	int i = 0;
	char name[50], id_char[3], password[50];
	int id_num;

	FILE* fptr;// Open the file and check the details
	fptr = fopen(file_name, "r");

	while (fscanf(fptr, " %49[^$]$%2[^$]$%d$%49[^\n]", name, id_char, &id_num, password) != EOF)
		(*count)++;

	fclose(fptr);

	*user = (system_users*)malloc(*count * sizeof(system_users));

	fptr = fopen(file_name, "r");

	while (fscanf(fptr, " %49[^$]$%2[^$]$%d$%49[^\n]", (*user)[i].user.name, (*user)[i].user.post, &(*user)[i].user.code, (*user)[i].pass) != EOF)
		i++;

	fclose(fptr);

}

void read_Data_activ(int* count_event, activities** user, int* count_catgry) {

	int i = 0;
	char line[100];
	char catgr[100];
	FILE* fptr;// Open the file and check the details
	fptr = fopen("Events.txt", "r");
	while (fgets(line, sizeof(line), fptr) != NULL)
		if (line[0] != '\n') {
			(*count_event)++;
		}
	fclose(fptr);

	*user = (activities*)malloc(*count_event * sizeof(activities));

	fptr = fopen("Events.txt", "r");

	while (fscanf(fptr, " %49[^$]$%2[^$]$%d$%d$%49[^$]$%2[^$]$%d$%d$%d$%d$%d$%d$%d$%29[^$]$%[^\n]", (*user)[i].event.name, (*user)[i].event.post, &(*user)[i].event.code, &(*user)[i].point, (*user)[i].prs_in_chrg.name, (*user)[i].prs_in_chrg.post, &(*user)[i].prs_in_chrg.code, &(*user)[i].date[0].day, &(*user)[i].date[0].month, &(*user)[i].date[0].year, &(*user)[i].date[1].day, &(*user)[i].date[1].month, &(*user)[i].date[1].year, (*user)[i].type, catgr) != EOF) {
		int cat_num = 0, sz = 0;
		char cpy_sent[50];
		strcpy(cpy_sent, catgr);

		// for get_num_of_cat function 
		*count_catgry = (*count_catgry) * 10;

		// Calculating the number of categories in the string
		char* calcu_cat = strtok(cpy_sent, "$");
		while (calcu_cat != NULL) {
			printf("", calcu_cat);
			calcu_cat = strtok(NULL, "$");
			sz++;
			(*count_catgry)++;
		}

		// Allocating memory to category array
		for (int cat_nm = 0; cat_nm < sz; cat_nm++) {
			(*user)[i].category[cat_nm] = (char*)calloc(30, sizeof(char));
		}

		// Seperate the categories and put them into the array in structure
		char* sep = strtok(catgr, "$");
		while (sep != NULL && cat_num < sz) {
			strcpy((*user)[i].category[cat_num], sep);
			sep = strtok(NULL, "$");
			cat_num++;
		}
		i++;

	}
	fclose(fptr);
}

int get_num_of_cat(int series_lgth, int num_evnt, int pst) {
	for (int del = 0; del < (num_evnt - (pst + 1)); del++) {
		series_lgth = series_lgth / 10;
	}

	if (series_lgth > 10) {
		series_lgth = series_lgth - (series_lgth / 10 * 10);
	}
	return series_lgth;
}

void login_track(char* userID) {

	char Record_login[100];
	time_t currentTime = time(NULL);
	struct tm* localTime;

	time(&currentTime);
	localTime = localtime(&currentTime);
	sprintf(Record_login, "%s %s", userID, asctime(localTime));
	//printf("Current local time:%s", Record_login);
	FILE* fptr_x = fopen("Record_Login.txt", "a");
	fprintf(fptr_x, "%s", Record_login);
	fclose(fptr_x);

}

int cd_valid(char cd[30]) {
	char role[4][3] = { "AD","ST","PL","PC" };
	int cft = 0;
	if (strlen(cd) != 7) {
		return -1;
	}
	for (int role_cd = 0; role_cd < 4; role_cd++) {
		if (strncmp(cd, role[role_cd], 2) == 0) {
			break;
		}
		else {
			cft++;
		}
	}
	if (cft == 4) {
		return -1;
	}
	for (int num_cd = 2; num_cd < 7; num_cd++) {
		if (!(cd[num_cd] >= '0' && cd[num_cd] <= '9')) {
			return -1;
		}
	}
	return 0;
}

int check_user(int* id_num) {

	int i, j, returned;

	char password[50], id[30], ch_id_num[6];

	char file_name[100];
	int count = 0;
	system_users* user = NULL;

	for (i = 0; i < 5; i++) {

		strcpy(id, "");
		strcpy(password, "");
		strcpy(ch_id_num, "");
		(*id_num) = 0;
		count = 0;
		user = NULL;

		printf("Enter User ID: ");
		scanf(" %29[^\n]", id);
		returned = cd_valid(id);
		printf("Enter Password: ");
		scanf(" %49[^\n]", password);

		if (returned == 0) {

			for (j = 2; j < 7; j++)
				ch_id_num[j - 2] = id[j];
			(*id_num) = atoi(ch_id_num);

			if (strncmp(id, "AD", 2) == 0) {
				strcpy(file_name, "Admin.txt");
				read_user_data(file_name, &count, &user);
			}
			else if (strncmp(id, "ST", 2) == 0) {
				strcpy(file_name, "Student.txt");
				read_user_data(file_name, &count, &user);
			}
			else if (strncmp(id, "PC", 2) == 0) {
				strcpy(file_name, "P_Coordinator.txt");
				read_user_data(file_name, &count, &user);
			}
			else if (strncmp(id, "PL", 2) == 0) {
				strcpy(file_name, "P_Leader.txt");
				read_user_data(file_name, &count, &user);
			}

			int L, R, mid;
			L = -1;
			R = count;
			while ((L + 1) != R) {
				mid = (L + R) / 2;
				if (user[mid].user.code <= *id_num)
					L = mid;
				else
					R = mid;
			}
			if (L >= 0 && *id_num == user[L].user.code && (strncmp(id, user[L].user.post, 2) == 0) && (strcmp(password, user[L].pass) == 0)) {

				login_track(id);

				if (strncmp(id, "AD", 2) == 0)
					return 1;
				else if (strncmp(id, "ST", 2) == 0)
					return 2;
				else if (strncmp(id, "PC", 2) == 0)
					return 3;
				else if (strncmp(id, "PL", 2) == 0)
					return 4;
			}
		}

		printf("   *User ID and Password not match*\n");
	}

	free(user);
	return -1;

}

void read_Student_point(int* count_student, point_mngmt** student) {

	FILE* fptr;

	fptr = fopen("Student_point_management.txt", "r");

	char line[1000];
	while (fgets(line, sizeof(line), fptr) != NULL)
		(*count_student)++;

	fclose(fptr);

	*student = (point_mngmt*)malloc(*count_student * sizeof(point_mngmt));

	fptr = fopen("Student_point_management.txt", "r");

	char module_event[900];
	int i = 0;
	while (fscanf(fptr, " %49[^$]$%2[^$]$%d$%d$%d$%d$%899[^\n]", (*student)[i].stdt_data.name, (*student)[i].stdt_data.post, &(*student)[i].stdt_data.code, &(*student)[i].point, &(*student)[i].module_count, &(*student)[i].event_count, module_event) != EOF) {

		if ((*student)[i].module_count != 0 || (*student)[i].event_count != 0) {

			int j;
			char* token;
			token = strtok(module_event, "$");

			if ((*student)[i].module_count > 0)
				(*student)[i].module_dn = (detail*)malloc((*student)[i].module_count * sizeof(detail));

			for (j = 0; j < (*student)[i].module_count; j++) {
				sscanf(token, "%49[^&]&%2[^&]&%d", (*student)[i].module_dn[j].name, (*student)[i].module_dn[j].post, &(*student)[i].module_dn[j].code);
				token = strtok(NULL, "$");
			}

			if ((*student)[i].event_count > 0)
				(*student)[i].evnt_dn = (detail*)malloc((*student)[i].event_count * sizeof(detail));

			for (j = 0; j < (*student)[i].event_count; j++) {
				sscanf(token, "%49[^&]&%2[^&]&%d", (*student)[i].evnt_dn[j].name, (*student)[i].evnt_dn[j].post, &(*student)[i].evnt_dn[j].code);
				token = strtok(NULL, "$");
			}

		}

		strcpy(module_event, "");
		i++;
	}

	fclose(fptr);

}

void read_Data_reqt(int* count, module** user) {

	int i = 0;
	char line[100];

	FILE* fptr;// Open the file and check the details
	fptr = fopen("Module.txt", "r");

	while (fgets(line, sizeof(line), fptr) != NULL)
		(*count)++;

	fclose(fptr);

	*user = (module*)malloc(*count * sizeof(module));

	fptr = fopen("Module.txt", "r");

	while (fscanf(fptr, " %49[^$]$%2[^$]$%d$%39[^$]$%d", (*user)[i].module.name, (*user)[i].module.post, &(*user)[i].module.code, (*user)[i].major, &(*user)[i].point) != EOF)
		i++;

	fclose(fptr);
}

void rmd(char* sen, int indct) {
	int lgth = strlen(sen);
	if (indct == 1) {
		printf("\n\n-------- Error: %s --------\n", sen);
	}
	else if (indct == 2) {
		printf("\n    **** %s ****    \n", sen);
	}
	else {
		printf("\n%s\n", sen);
	}
}

char* get_Smember(activities** lst, int elem, char member[3]) {
	if (strcmp(member, "1") == 0) {
		return (*lst)[elem].event.name;
	}
	else if (strcmp(member, "3") == 0) {
		return (*lst)[elem].prs_in_chrg.name;
	}
	else if (strcmp(member, "7") == 0) {
		return (*lst)[elem].type;
	}
	else {
		char sen[30];
		strcpy(sen, "Detail not found");
		rmd(sen, 1);
		return NULL;
	}
}

int search_string(activities** Check_list, char* to_Check, int** search_result, int* count_event, char chce[3]) {
	char* mapping;
	int mtched_lctor = 0;

	(*search_result) = (int*)malloc((*count_event) * sizeof(int));

	for (int elem = 0; elem < (*count_event); elem++) {
		// retain the original string for checking
		// get_Smember return the specific member of structure for checking
		mapping = (char*)malloc((strlen(get_Smember(Check_list, elem, chce)) * sizeof(char) + 1));
		strcpy(mapping, (get_Smember(Check_list, elem, chce)));

		//check for the level of similarity
		int num_simi = 0;

		for (int chr_Search = 0; chr_Search < strlen(to_Check); chr_Search++) {
			// ---------------------------------------------------------- first character of keyword
			mapping = strchr((mapping), to_Check[chr_Search]);
			if (mapping == NULL) {
				break;
			}
			else {
				num_simi++;
			}
			*(mapping++); //---------------------------------------- to skip the checked character to the next to avoid endless loop
		}
		if (num_simi == strlen(to_Check)) {
			(*search_result)[mtched_lctor] = elem; //------------------ append the location/index of the matched string to the list 
			mtched_lctor++;
		}
		mapping = NULL;
		free(mapping);
	}
	(*search_result) = (int*)realloc((*search_result), (mtched_lctor * sizeof(int)));
	return mtched_lctor;
}

// Return the member of structure based user's choice (String)

int date_valid(int dy, int mnth, int yr) {

	char sen[30];

	if (yr >= 1900 && yr <= 9999 || yr == 0) {
		//check month
		if (mnth >= 0 && mnth <= 12) {
			//check days
			if ((dy >= 0 && dy <= 31) && (mnth == 1 || mnth == 3 || mnth == 5 || mnth == 7 || mnth == 8 || mnth == 10 || mnth == 12))
				return 0;
			else if ((dy >= 0 && dy <= 30) && (mnth == 4 || mnth == 6 || mnth == 9 || mnth == 11))
				return 0;
			else if ((dy >= 0 && dy <= 28) && (mnth == 2))
				return 0;
			else if (dy == 29 && mnth == 2 && (yr % 400 == 0 || (yr % 4 == 0 && yr % 100 != 0)))
				return 0;
			else if (mnth == 0 && (dy >= 0 && dy <= 31))
				return 0;
			else{

				strcpy(sen, "Day is not valid.");
				rmd(sen, 1);
			}
			return -1;
		}
		else {
			strcpy(sen, "Month is not valid.");
			rmd(sen, 1);
			return -1;
		}
	}
	else {
		strcpy(sen, "Year is not valid.");
		rmd(sen, 1);
		return -1;
	}
}

int inp_date_vld(char date_ent[11], int* dy, int* mnth, int* yr) {
	int dash = 0;
	char strg[11];
	char sen[30];
	strcpy(strg, date_ent);
	for (int chr = 0; chr < strlen(strg); chr++) {
		if (strg[chr] == '/') {
			dash++;
		}
		else if (!(strg[chr] >= '/' && strg[chr] <= '9')) {
			strcpy(sen, "Invalid character");
			rmd(sen, 1);
			return -1;
		}
	}
	if (dash != 2) {
		strcpy(sen, "Incomplete Date");
		rmd(sen, 1);
		return -1;
	}
	sscanf(date_ent, "%d/%d/%d", dy, mnth, yr);
	if ((date_valid((*dy), (*mnth), (*yr))) == 0) {
		return 0;
	}
	else {
		return -1;
	}
}

// Return the member of structure based user's choice (Integer)
int get_Imember(activities** lst, int elem, char member[3]) {

	char sen[30];

	if (strcmp(member, "2") == 0) {
		return (*lst)[elem].event.code;
	}
	else if (strcmp(member, "4") == 0) {
		return (*lst)[elem].prs_in_chrg.code;
	}
	else {
		strcpy(sen, "Detail not found");
		rmd(sen, 1);
		return 0;
	}
}

int search_string_ctgry(activities** Check_list, char* to_Check, int** search_result, int ctgry_sries, int* count_event, char chce[3]) {
	char* mapping;
	int mtched_lctor = 0;

	(*search_result) = (int*)malloc((*count_event) * sizeof(int));
	for (int elem = 0; elem < (*count_event); elem++) {
		// retain the original string for checking
		// get_Smember return the specific member of structure for checking
		for (int evt_cat = 0; evt_cat < (get_num_of_cat(ctgry_sries, (*count_event), elem)); evt_cat++) {
			mapping = (char*)malloc((strlen((*Check_list)[elem].category[evt_cat])) * sizeof(char) + 1);
			strcpy(mapping, ((*Check_list)[elem].category[evt_cat]));

			//check for the level of similarity
			int num_simi = 0;

			for (int chr_Search = 0; chr_Search < strlen(to_Check); chr_Search++) {
				// ---------------------------------------------------------- first character of keyword
				mapping = strchr((mapping), to_Check[chr_Search]);
				if (mapping == NULL) {
					break;
				}
				else {
					num_simi++;
				}
				*(mapping++); //---------------------------------------- to skip the checked character to the next to avoid endless loop
			}
			if (num_simi == strlen(to_Check)) {
				(*search_result)[mtched_lctor] = elem; //------------------ append the location/index of the matched string to the list 
				mtched_lctor++;
			}
			mapping = NULL;
			free(mapping);
		}
	}
	(*search_result) = (int*)realloc((*search_result), (mtched_lctor * sizeof(int)));
	return mtched_lctor;
}

void read_Data_std_dtl(int* count_event, std_dtl** user, int* count_catgry) {

	int i = 0;
	char line[100];
	char catgr[100];
	FILE* fptr;// Open the file and check the details
	fptr = fopen("Student_details.txt", "r");
	while (fgets(line, sizeof(line), fptr) != NULL)
		if (line[0] != '\n') {
			(*count_event)++;
		}
	fclose(fptr);

	*user = (std_dtl*)malloc(*count_event * sizeof(std_dtl));

	fptr = fopen("Student_details.txt", "r");

	while (fscanf(fptr, " %d$%29[^$]$%[^\n]", &(*user)[i].std_cd, (*user)[i].major, catgr) != EOF) {
		int cat_num = 0, sz = 0;
		char cpy_sent[50];
		strcpy(cpy_sent, catgr);

		// for get_num_of_cat function 
		*count_catgry = (*count_catgry) * 10;

		// Calculating the number of categories in the string
		char* calcu_cat = strtok(cpy_sent, "$");
		while (calcu_cat != NULL) {
			printf("", calcu_cat);
			calcu_cat = strtok(NULL, "$");
			sz++;
			(*count_catgry)++;
		}

		// Allocating memory to category array
		for (int cat_nm = 0; cat_nm < sz; cat_nm++) {
			(*user)[i].category[cat_nm] = (char*)calloc(30, sizeof(char));
		}

		// Seperate the categories and put them into the array in structure
		char* sep = strtok(catgr, "$");
		while (sep != NULL && cat_num < sz) {
			strcpy((*user)[i].category[cat_num], sep);
			sep = strtok(NULL, "$");
			cat_num++;
		}
		i++;

	}
	fclose(fptr);
}

// Sorting with location
void srt(int*** list, int ttl, int srt_by, int lgth) {
	int main_idx;
	int temp[3];
	for (int crrt = 0; crrt < (ttl - 1); crrt++) {
		main_idx = crrt;
		for (int next = crrt + 1; next < ttl; next++) {
			if ((*list)[next][srt_by] > (*list)[main_idx][srt_by]) {
				main_idx = next;
			}
		}
		if (main_idx != crrt) {
			// Copy total items +2 for first two elements
			for (int cpy = 0; cpy < ((*list)[crrt][lgth] + 2); cpy++) {
				temp[cpy] = (*list)[crrt][cpy];
			}
			for (int cpy = 0; cpy < ((*list)[main_idx][lgth] + 2); cpy++) {
				(*list)[crrt][cpy] = (*list)[main_idx][cpy];
			}
			for (int cpy = 0; cpy < ((*list)[crrt][lgth] + 2); cpy++) {
				(*list)[main_idx][cpy] = temp[cpy];
			}
		}
	}
}

int sum(int series, int total) {
	if (series < 10) {
		return series + total;
	}
	else {
		total = total + (series % 10);
		return sum(series / 10, total);
	}
}

int days_cal(int strt_day, int strt_mnth, int strt_yr, int en_day, int en_mnth, int en_yr) {
	int ttl_day = 0;
	int days = 0;
	for (;;) {
		if ((strt_mnth == en_mnth) && (strt_yr == en_yr)) {
			if (ttl_day == 0) {
				strt_day = en_day - strt_day + 1;
			}
			else {
				strt_day = en_day - strt_day;
			}
			ttl_day += strt_day;
			return ttl_day;
		}
		if (strt_mnth == 1 || strt_mnth == 3 || strt_mnth == 5 || strt_mnth == 7 || strt_mnth == 8 || strt_mnth == 10 || strt_mnth == 12) {
			strt_day = 31 - strt_day + 1;
			if (strt_mnth == 12 && strt_day == 31) {
				ttl_day = ttl_day + strt_day;
				strt_mnth = 1;
				strt_day = 1;
				strt_yr++;
			}
			else {
				ttl_day += strt_day;
				strt_mnth++;
				strt_day = 1;
			}

		}
		else if (strt_mnth == 4 || strt_mnth == 6 || strt_mnth == 9 || strt_mnth == 11) {
			strt_day = 30 - strt_day + 1;
			ttl_day += strt_day;
			strt_mnth++;
			strt_day = 1;
		}
		else if (strt_mnth == 2 && (strt_yr % 400 == 0 || (strt_yr % 4 == 0 && strt_yr % 100 != 0))) {
			strt_day = 29 - strt_day + 1;
			ttl_day += strt_day;
			strt_mnth++;
			strt_day = 1;
		}
		else if (strt_mnth == 2) {
			strt_day = 28 - strt_day + 1;
			ttl_day += strt_day;
			strt_mnth++;
			strt_day = 1;
		}
	}
}

int pass_validate(char* pss) {
	char cmmt[5][50] = { "Should be at least 10 Characters","Should Contain At Least 1 Uppercase Letter","Should Not Contain Whitespace","Should Contain Special Character","Should Contain Number" };// BUFFER+10
	char usr_pss[50];
	int counter[5] = { 0,0,0,0,0 };
	int vlded = 0;
	// 0 = range, 1 = E, 2= space, 3= #, 4 = 23

	strcpy(usr_pss, pss);
	if (strlen(usr_pss) < 10) {
		counter[0] = strlen(usr_pss);
	}
	for (int chr = 0; chr < strlen(usr_pss); chr++) {

		if (usr_pss[chr] == ' ') {
			counter[2] = strlen(usr_pss);
			counter[3] += 1;
		}
		if (!(usr_pss[chr] >= 'A' && usr_pss[chr] <= 'Z'))
			counter[1] += 1;
		if (!(usr_pss[chr] >= '0' && usr_pss[chr] <= '9'))
			counter[4] += 1;
		if (usr_pss[chr] >= '!' && usr_pss[chr] <= '}') {
			if (usr_pss[chr] >= 'A' && usr_pss[chr] <= 'Z')
				counter[3] += 1;
			if (usr_pss[chr] >= '0' && usr_pss[chr] <= '9')
				counter[3] += 1;
			if (usr_pss[chr] >= 'a' && usr_pss[chr] <= 'z')
				counter[3] += 1;
		}

	}
	printf("\n");
	for (int chk = 0; chk < 5; chk++) {
		if (counter[chk] != strlen(usr_pss)) {
			printf("[0]-----");

		}
		else {
			printf("[X]-----");
			vlded += 1;
		}
		printf("%s\n", cmmt[chk]);
	}
	// rmd("Invalid Password Format",1)
	printf("\n");
	if (vlded == 0) {
		return 0;
	}
	else {
		return -1;
	}
}

int nm_Validate(char* nm) {
	//nm = lower_str(nm);
	for (int chr = 0; chr < strlen(nm); chr++) {
		if (nm[chr] == ' ') {
			continue;
		}
		//--------------------------------------------------------------------Checking digit in string---------------
		if (isdigit(nm[chr]) != 0) {
			printf("-------- Error: Name should not contain NUMERICAL CHARACTER. --------\n");
			return -1;
			//--------------------------------------------------------------------Checking special.c in string-----------
		}
		else if (isalpha(nm[chr]) == 0) {
			printf("-------- Error: Name should not contain SPECIAL CHARACTER. --------\n");
			return -1;
		}
	}
	return 0;
}

int GTI(char userinput[100]) {
	//return 0==ID -1==password 1==Name
	char userid[100] = "", userlvl[100] = "";
	int IDnuvali = 0, passvali = 0, namevali = 0;

	for (int i = 0; userinput[i] != '\0'; i++) {
		if (ispunct(userinput[i]))
			return -1;//password type;
	}

	for (int start = 0; start < 2; start++) {
		if (isupper(userinput[start])) {
			strncat(userlvl, &userinput[start], 1);
			IDnuvali += 1;
		}
	}

	for (int start = 2; start < 7; start++) {
		strncat(userid, &userinput[start], 1);
		IDnuvali += 1;
	}
	if (IDnuvali == 7) {
		return 0;//ID type
	}
	if (passvali == 0)
		return 1;//username type

}

int assignID(int data_Placed, char data_Type[3]) {
	int tempID;
	char txt_ID[100];
	FILE* fptr;
	fptr = fopen("system_capacity.txt", "r");
	fgets(txt_ID, sizeof(txt_ID), fptr);
	char* TOKEN = strtok(txt_ID, "$");
	int txt_line[100];

	for (int i = 0; i < 6; i++)//compare with the information separate with '$' with another info
	{
		int int_TOKEN = atoi(TOKEN);
		txt_line[i] = int_TOKEN;
		if (i == data_Placed) {//chose the right column and +1 to become a new TP number for user
			txt_line[i] = ++int_TOKEN;//+1 at the position
			printf("Your ID is :%s%i", data_Type, txt_line[i]);
			tempID = txt_line[i];
		}
		TOKEN = strtok(NULL, "$");
	}
	fclose(fptr);

	FILE* fptr1;
	fptr1 = fopen("system_capacity.txt", "w");
	for (int y = 0; y < 6; y++)
		fprintf(fptr1, "%i$", txt_line[y]);
	fclose(fptr1);
	return tempID;
}

void changePMF(int uID, char uNAME[100]) {

	int i, j;

	int count_student = 0;
	point_mngmt* student = NULL;

	read_Student_point(&count_student, &student);

	FILE* fptr_z = fopen("Student_point_management.txt", "w");


	for (i = 0; i < count_student; i++) {
		if (uID == student[i].stdt_data.code)
			strcpy(student[i].stdt_data.name, uNAME);
		fprintf(fptr_z, "%s$%s$%i$%i$%i$%i", student[i].stdt_data.name, student[i].stdt_data.post, student[i].stdt_data.code, student[i].point, student[i].module_count, student[i].event_count);

		if (student[i].module_count > 0) {
			for (j = 0; j < student[i].module_count; j++) {
				fprintf(fptr_z, "$%s&%s&%i", student[i].module_dn[j].name, student[i].module_dn[j].post, student[i].module_dn[j].code);
			}
		}

		if (student[i].event_count > 0) {
			for (j = 0; j < student[i].event_count; j++) {
				fprintf(fptr_z, "$%s&%s&%i", student[i].evnt_dn[j].name, student[i].evnt_dn[j].post, student[i].evnt_dn[j].code);
			}
		}

		printf("\n");
		fprintf(fptr_z, "\n");

		if (student[i].module_count > 0)
			free(student[i].module_dn);

		if (student[i].event_count > 0)
			free(student[i].evnt_dn);
	}
	fclose(fptr_z);

	free(student);

}

void read(char* filename, int* count, struct EDusers** fuser) {
	*count = 0;
	char x[100];
	FILE* fptr_y;
	fptr_y = fopen(filename, "r");

	while ((fgets(x, sizeof(x), fptr_y)) != NULL)
	{
		(*count)++;
	}

	*fuser = (struct EDusers*)malloc(*count * sizeof(struct EDusers));
	fseek(fptr_y, 0, SEEK_SET);

	for (int i = 0; i < *count; i++)
	{
		fgets(x, sizeof(x), fptr_y);
		sscanf(x, "%99[^$]$%2s$%5d$%99[^\n]", (*fuser)[i].fname, (*fuser)[i].ftype, &(*fuser)[i].fID, (*fuser)[i].fpassword);
	}//scanf the format form txt

	fclose(fptr_y);
}

void track_login() {
	char opt[][40] = { "ALL","Recent", "ID","RETURN" };
	char title[50];
	strcpy(title, "Admin-track");
	tablelise(title, opt, 4);
	//case 1
	char x[100];
	int count = 0, number_r;
	//case 2
	char userCode[8] = "";
	char fileCode[8] = "";
	FILE* fptr_6 = fopen("Record_Login.txt", "r");
	int Tinput;
	printf("Insert :"); scanf("%i", &Tinput);
	switch (Tinput) {
	case 1:
		rewind(fptr_6);
		printf("------------list-----------------\n");
		while ((fgets(x, sizeof(x), fptr_6)) != NULL) {
			printf("%s", x);

		}
		break;
	case 2:

		while ((fgets(x, sizeof(x), fptr_6)) != NULL) {
			count++;
		}

		printf("Total login :%i\n", count);
		do {
			printf("Insert the number of track in recent(max %i):", count);
			scanf("%i", &number_r);
		} while (number_r >= count);

		rewind(fptr_6);

		for (int i = 0; i < count; i++) {
			fgets(x, sizeof(x), fptr_6);
			if ((count - i) == number_r)
				printf("------------list-----------------\n");
			if ((count - i) <= number_r)
				printf("%s", x);
		}
		break;
	case 3:
		rewind(fptr_6);
		printf("search user:");
		scanf("%7s", userCode);

		printf("------------list-----------------\n");
		while (fgets(x, sizeof(x), fptr_6) != NULL) {
			strncpy(fileCode, x, 7);
			if (strcmp(userCode, fileCode) == 0)
				printf("%s", x);
		}
		break;
	default:
		printf("\n--Return--\n");
	}
}

void read_PL(char* filename, int* count, struct users_2** user)
{
	*count = 0;
	char x[100];
	FILE* fptr_y;
	fptr_y = fopen(filename, "r");

	if (fptr_y == NULL)
	{
		perror("Error opening file");
		exit(1);
	}

	while ((fgets(x, sizeof(x), fptr_y)) != NULL)
	{
		(*count)++;
	}

	printf("count: %i\n", *count);

	*user = (struct users_2*)malloc(*count * sizeof(struct users_2));
	fseek(fptr_y, 0, SEEK_SET);

	for (int i = 0; i < *count; i++)
	{
		fgets(x, sizeof(x), fptr_y);
		sscanf(x, "%99[^$]$%2s$%5d$%99[^\n]", (*user)[i].name, (*user)[i].type, &(*user)[i].ID, (*user)[i].password);
	}

	fclose(fptr_y);
}

char* lower_str(char* s) {
	for (int i = 0; i < strlen(s); i++) {
		if (s[i] < 'Z' && s[i]>'A') {
			s[i] += 'a' - 'A';
		}
	}
	return s;
}

void read_Data_module(int* count, module** user) {

	int i = 0;
	char line[100];

	FILE* fptr;// Open the file and check the details
	fptr = fopen("Module.txt", "r");

	while (fgets(line, sizeof(line), fptr) != NULL)
		(*count)++;

	fclose(fptr);

	*user = (module*)malloc(*count * sizeof(module));

	fptr = fopen("Module.txt", "r");

	while (fscanf(fptr, " %49[^$]$%2[^$]$%d$%39[^$]$%d", (*user)[i].module.name, (*user)[i].module.post, &(*user)[i].module.code, (*user)[i].major, &(*user)[i].point) != EOF)
		i++;
	fclose(fptr);
}

void inputDate(int* day, int* month, int* year, int* start_day, int* start_month, int* start_year, int* end_day, int* end_month, int* end_year) {
	int validInput = 0;
	int i;
	char start_date[12];
	char end_date[12];

	while (!validInput) {
		do {
			printf("Please enter event start date in the format dd/mm/yyyy: \n");
			scanf("%s", start_date);
		} while (inp_date_vld(start_date, start_day, start_month, start_year) == -1);
		validInput = 1;
	}
	validInput = 0;
	while (!validInput) {
		do {
			printf("Please enter event end date in the format dd/mm/yyyy: \n");
			scanf("%s", end_date);
		} while (inp_date_vld(end_date, end_day, end_month, end_year) == -1);

		// Check if the start date is before the end date
		if ((start_year <= end_year) &&
			(start_month <= end_month) &&
			(start_day < end_day)) {
			validInput = 1;
		}
		else {
			printf("End date must be later than start date.\n");

		}
		validInput = 1;
	}
	printf("Start date: %d/%d/%d\n", *start_day, *start_month, *start_year);
	printf("End date: %d/%d/%d\n", *end_day, *end_month, *end_year);
}

void AddingActivities(system_users** user, activities** evnt, int UI) {

	char UI_str[20];

	char opt[4][40] = { "Adding Modules to file", "Assign Events to Programme Leader", "Return", "Logout" };
	char title[50] = "Adding Activities";
	tablelise(title, opt, 4);

	char type[6][BUFFER_2] = { "Competition","Seminar","Talk","Workshop","Festival","Sports" };
	char major[8][BUFFER_2] = { "Engineering","Health Professions", "Arts & Design", "Philosophy", "Foreign Language", "Actuarial Science", "Finance" };
	char ctgry[7][BUFFER_2] = { "Motivational","IT/Techonology","Business","Musics","Psychology","Future Trend","Health" };

	char Module_post[3] = "MD";
	char PL_post[3] = "PL";
	char Event_post[3] = "EV";

	int start_day, start_month, start_year;
	int end_day, end_month, end_year;

	int count, i;
	int j;
	int ev_num;
	int validLeader;

	char PCFILE[50] = "Events.txt";

	count = 0;
	char MYFILE[50] = "P_Leader.txt";
	struct users_2* PL = NULL;
	read_PL(MYFILE, &count, &PL);

	do {
		printf("Choose your option:");
		scanf(" %d", &UI);
		printf("You selected option %d:", UI);
		_itoa(UI, UI_str, 10);
	} while ((ch_valid(UI_str, 1, 4)) == -1);

	//UI = choice_validation(1,4);
	switch (UI) {
	case 1:
		FILE* file;
		count = 0;
		file = fopen("Module.txt", "a");
		if (file == NULL) {
			printf("Failed to open file.");
		}

		printf("\nPlease enter the number of majors to add: ");
		scanf("%i", &count);
		for (i = 0; i < count; i++) {
			char module_nm[40];
			printf("\nEnter module name: ");
			scanf("%s", lower_str(module_nm));

			char major[40];
			printf("\nEnter major name: ");
			scanf("%s", lower_str(major));

			int point = 30;

			fprintf(file, "%s$%s$%d$%s$%d\n", module_nm, "MD", assignID(5, Module_post), major, point);
		}

		fclose(file);

		break;
	case 2:
		printf("\nProgram leader ");
		char event_name[40];
		char user_input1[40], user_input2[40], user_input3[40];
		ev_num = 0;
		j = 0;
		int day, month, year;
		start_day = 0, start_month = 0, start_year = 0;
		end_day = 0, end_month = 0, end_year = 0;
		int points;
		FILE* fptr;
		fptr = fopen(PCFILE, "a");
		if (fptr == NULL) {
			printf("Failed to open file.");
		}

		printf("Please enter the number of events to add: ");
		scanf("%i", &ev_num);
		for (int i = 0; i < ev_num; i++) {
			printf("Please enter event name: ");
			scanf("%s", event_name);
			lower_str(event_name);

			printf("The program leaders available are: \n");
			for (int i = 0; i < count; i++)
			{
				printf("\nname: %s", PL[i].name);
				printf("\ntype: %s", PL[i].type);
				printf("\nID: %d", PL[i].ID);
				printf("\n----------------------------\n");
			}

			validLeader = 0;

			while (validLeader == 0) {
				printf("Please enter the name of the program leader: ");
				char leaderName[50];
				scanf(" %49[^\n]", leaderName);

				// Check if the entered name exists in the list of available program leaders
				for (int i = 0; i <= count; i++) {
					if (strcmp(leaderName, PL[i].name) == 0) {
						validLeader = 1; // A valid leader name is found
						break;
					}
					if (i == count && validLeader == 0) {
						printf("\nInvalid program leader name. Please try again.\n");
					}
				}
			}

			int validPoint = 0;

			while (validPoint == 0) {
				printf("\nPlease enter event's points: ");
				scanf("%i", &points);
				if (points >= MIN_POINTS && points <= MAX_POINTS) {
					printf("Number of points is valid.");
					++validPoint;
				}
				else {
					printf("Number of points for event must be between 1 and 15.Please try again.\n");

				}
			}

			inputDate(&day, &month, &year, &start_day, &start_month, &start_year, &end_day, &end_month, &end_year);

			printf("Enter the event type(Competition,Seminar,Talk,Workshop,Festival,Sports): ");
			scanf("%s", user_input1);
			// Convert user input to lowercase for case-insensitive comparison
			lower_str(user_input1);

			int found = 0;

			//iterate through each row in the array
			for (int i = 0; i < 6; i++) {
				//Compare user input with each element
				if (strcmp(user_input1, type[i]) == 0) {
					found = -1;
					printf("Event type is found.\n");
					break;
				}
			}
			if (found != -1) {
				printf("Input does not match any event category.\n");
			}

			printf("Enter the event category(Motivational,IT/Techonology,Business,Musics,Psychology,Future Trend,Health): ");
			scanf("%s", user_input2);
			lower_str(user_input2);

			//iterate through each row in the array
			for (int i = 0; i < 7; i++) {
				//Compare user input with each element
				if (strcmp(user_input2, ctgry[i]) == 0) {
					found = -1;
					printf("Event category is found.\n");
					break;
				}
			}
			if (found != -1) {
				printf("Input does not match any event category.\n");
			}

			printf("Enter the major(Engineering,Health Professions,Arts & Design,Philosophy,Foreign Language,Actuarial Science, Finance: ");
			scanf("%s", user_input3);
			lower_str(user_input3);

			//iterate through each row in the array
			for (int i = 0; i < 8; i++) {
				//Compare user input with each element
				if (strcmp(user_input3, major[i]) == 0) {
					found = -1;
					printf("Major is found.");
					break;
				}
			}
			if (found != -1) {
				printf("Input does not match any major.\n");
			}

			fprintf(fptr, "%s$%s$%d$%d$%s$%s$%d$%d$%d$%d$%d$%d$%d$%s$%s$%s\n", event_name, "EV", assignID(4, Event_post), points, PL[j].name, "PL", PL[j].ID, start_day, start_month, start_year, end_day, end_month, end_year, type, ctgry, major);
		
		}
	case 3:
		printf("Returning to main menu...\n");
		break;
	case 4:
		printf("Thank you for using Programme Coordinator Menu.\n");
		exit(0);
	}
}

int UpdatingActivities(activities** user, int count_event) {

	activities* firstptr = NULL;

	FILE* inputFile, * outputFile;
	char line[1000];
	char* token;
	int UI;
	char UI_str[20];
	int i;
	char oldprs[20], oldevnm[20], oldevsday[3], oldevsmonth[3], oldevsyear[5], oldeveday[3], oldevemonth[3], oldeveyear[5];
	int index;

	// Allocate memory for the array of activities
	int numActivities = 8;
	firstptr = (activities*)malloc(numActivities * sizeof(activities));

	if (firstptr == NULL) {
		perror("Memory allocation error");
		return 1;
	}

	inputFile = fopen("Events.txt", "r");
	if (inputFile == NULL) {
		perror("Error opening file");
		return 1;
	}
	//until here ckecking
	printf("Enter the event name to replace: ");
	getc(stdin);
	fgets(oldevnm, sizeof(oldevnm), stdin);
	oldevnm[strcspn(oldevnm, "\n")] = '\0';  // Remove the newline character at the end

	for (i = 0; i < count_event; i++) {
		if (strcmp((*user)[i].event.name, oldevnm) == 0) {
			printf("Event is found!");
			index = i;
			break;
		}
		if (i == count_event) {
			printf("Event is not found.\n");
			return 1;
		}
	}

	printf("Enter the new substring: ");
	scanf("%s", (*user)[index].event.name);

	outputFile = fopen("temp_output_file.txt", "w");
	if (outputFile == NULL) {
		perror("Error creating/opening output file");
		return 1;
	}

	while (fgets(line, sizeof(line), inputFile) != NULL) {
		// Tokenize the line using "$" as a delimiter
		token = strtok(line, "$");

		while (token != NULL) {
			// Check if the current token is the old substring
			if (strcmp(token, oldevnm) == 0) {
				// Replace the old substring with the new one
				fprintf(outputFile, "%s", (*user)[i].event.name);

				char opt[9][40] = { "Programme leader", "Start day", "Start month", "Start year", "End day", "End month","End year","Return.","Logout" };
				char title[50] = "Update Activities";
				tablelise(title, opt, 9);
				do {
					printf("Choose your option: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 9)) == -1);

				switch (UI) {
				case 1: {
					printf("Please enter the programme leader's name to update: ");
					scanf("%s", oldprs);
					if (strcmp(token, oldprs) == 0) {
						// Replace the old substring with the new one
						fprintf(outputFile, "%s", (*user)[i].prs_in_chrg.name);
					}
					else {
						printf("Name is not found.");
						fprintf(outputFile, "%s", token);   //If not the old substring, keep token as the same
					}
					fprintf(outputFile, "$");   //Print delimiter back("$")
					token = strtok(NULL, "$");  //Move to next token
					break;
				}
				case 2: {
					printf("Please enter the event start day to update: ");
					scanf("%s", oldevsday);
					if (strcmp(token, oldevsday) == 0) {
						// Replace the old substring with the new one
						fprintf(outputFile, "%s", (*user)[i].date[0].day);
					}
					else {
						printf("Name is not found.");
						fprintf(outputFile, "%s", token);
					}

					fprintf(outputFile, "$");
					token = strtok(NULL, "$");
					break;
				}
				case 3: {
					printf("Please enter the event start month to update: ");
					scanf("%s", oldevsmonth);
					if (strcmp(token, oldevsmonth) == 0) {
						// Replace the old substring with the new one
						fprintf(outputFile, "%s", (*user)[i].date[0].month);
					}
					else {
						printf("Name is not found.");
						fprintf(outputFile, "%s", token);
					}

					fprintf(outputFile, "$");
					token = strtok(NULL, "$");
					break;
				}
				case 4: {
					printf("Please enter the event start year to update: ");
					scanf("%s", oldevsyear);
					if (strcmp(token, oldevsyear) == 0) {
						// Replace the old substring with the new one
						fprintf(outputFile, "%s", (*user)[i].date[0].year);
					}
					else {
						printf("Name is not found.");
						fprintf(outputFile, "%s", token);
					}

					fprintf(outputFile, "$");
					token = strtok(NULL, "$");
					break;
				}
				case 5: {
					printf("Please enter the event end day to update: ");
					scanf("%s", oldeveday);
					if (strcmp(token, oldeveday) == 0) {
						// Replace the old substring with the new one
						fprintf(outputFile, "%s", (*user)[i].date[1].day);
					}
					else {
						printf("Name is not found.");
						fprintf(outputFile, "%s", token);
					}

					fprintf(outputFile, "$");
					token = strtok(NULL, "$");
					break;
				}
				case 6: {
					printf("Please enter the event end month to update: ");
					scanf("%s", oldevemonth);
					if (strcmp(token, oldevemonth) == 0) {
						// Replace the old substring with the new one
						fprintf(outputFile, "%s", (*user)[i].date[1].month);
					}
					else {
						printf("Name is not found.");
						fprintf(outputFile, "%s", token);
					}

					fprintf(outputFile, "$");
					token = strtok(NULL, "$");
					break;
				}
				case 7: {
					printf("Please enter the end year to update: ");
					scanf("%s", oldeveyear);
					if (strcmp(token, oldeveyear) == 0) {
						// Replace the old substring with the new one
						fprintf(outputFile, "%s", (*user)[i].date[1].year);
					}
					else {
						printf("Name is not found.");
						fprintf(outputFile, "%s", token);
					}

					fprintf(outputFile, "$");
					token = strtok(NULL, "$");
					break;
				}
				case 8: {
					printf("Returning to main menu...");
					break;
				}
				case 9: {
					printf("Thank you for using Programme Coordinator Menu.\n");
					return 0;
				}
				}
				fclose(inputFile);
				fclose(outputFile);

			}
			else {
				fprintf(outputFile, "%s", token);
				token = strtok(NULL, "$");
			}
			// Move to the next line
			fprintf(outputFile, "\n");
		}
	}

	// Close the files
	fclose(inputFile);
	fclose(outputFile);
	free(firstptr);

	// Rename the new file to the original file name
	remove("Events.txt");
	rename("temp_output_file.txt", "Events.txt");

	printf("Replacement completed successfully.\n");

	return 0;
}

#endif