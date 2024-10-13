#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "small_function.h"
#define BUFFER 40
#define LAYOUT 50
#define MYFILE "P_Leader.txt"
#define PCFILE "Events.txt"
#define SYSFILE "system_capacity.txt"
#define MIN_POINTS 1
#define MAX_POINTS 15
#define BUFFER_2 256

void login_logout_menu();
void student_menu(int id_num);
void manage_activities(int id_num);
void view_track_activities_credits(int id);
void register_student_activities(int id);
void unregister_student_activities(int id);
void Programme_leader(int lg_cd);
void Admin();
int Programme_coordinator();

int main() {

	login_logout_menu();

	return 0;

}

void login_logout_menu() {

	int choice, returned;

	for (;;) {

		char opt[2][40] = { "Login", "Exit" };
		tablelise("Student Semester Planner", opt, 2);

		choice = choice_validation(1, 2);
		printf("\n");

		int id_num;

		switch (choice) {
		case 1:
			returned = check_user(&id_num);
			switch (returned) {
			case -1:
				break;
			case 1:
				Admin();
				break;
			case 2:
				student_menu(id_num);
				break;
			case 3:
				Programme_coordinator();
				break;
			case 4:
				Programme_leader(id_num);
				break;
			}
			break;
		case 2:
			printf("\n   *Thank You for using Student Semester Planner*\n");
			exit(0);
		}
	}
}

//----------Student----------//
void student_menu(int id_num) {

	char username[50];
	int returned, choice = 0;

	do {
		char opt[3][40] = { "Logout", "Manage Activities", "View/Track Activities and Credits" };
		tablelise("Student Menu", opt, 3);

		choice = choice_validation(1, 3);

		switch (choice) {
		case 1:
			break;
		case 2:
			manage_activities(id_num);
			break;
		case 3:
			view_track_activities_credits(id_num);
			break;
		}
	} while (choice != 1);
}

void manage_activities(int id_num) {

	char opt[3][40] = { "Back", "Register Activities", "Unregister Activities" };
	int choice;

	do {
		tablelise("Student Menu", opt, 3);
		choice = choice_validation(1, 3);

		switch (choice) {
		case 1:
			break;
		case 2:
			register_student_activities(id_num);
			break;
		case 3:
			unregister_student_activities(id_num);
			break;
		}
	} while (choice != 1);

}

void register_student_activities(int id) {

	int count_student = 0;
	point_mngmt* student = NULL;
	read_Student_point(&count_student, &student);

	int count_event = 0;
	int count_category = 0;
	activities* firstptr = NULL;
	read_Data_activ(&count_event, &firstptr, &count_category);

	int count_module = 0;
	module* modules = NULL;
	read_Data_reqt(&count_module, &modules);

	int choice, i, j;

	int L, R, mid;
	L = -1;
	R = count_student;

	while ((L + 1) != R) {
		mid = (L + R) / 2;
		if (student[mid].stdt_data.code <= id)
			L = mid;
		else
			R = mid;
	}
	if (L >= 0 && student[L].stdt_data.code == id) {

		char opt[3][40] = { "Back", "Module", "Event" };
		tablelise("Register Activities", opt, 3);
		choice = choice_validation(1, 3);

		if (choice == 2) {

			int index = 0;
			int num = 0;

			if (student[L].module_count != count_module) {

				int* location;
				location = (int*)calloc(count_module - student[L].module_count, sizeof(int));

				printf("\nModule List\n\n1. Return to Management Activities\n\n");

				for (i = 0; i < count_module; i++) {

					if (student[L].module_count != 0) {

						if (student[L].module_dn[index].code != modules[i].module.code) {

							printf("%d.\n", num+2);
							printf("Module name: %s\nModule code: %s%d\nModule major: %s\nPoint Carried: %d", modules[i].module.name, modules[i].module.post, modules[i].module.code, modules[i].major, modules[i].point);
							printf("\n\n");

							location[num] = i;
							num++;

						}

						else
							index++;

					}

					else {

						printf("%d.\n", num + 2);
						printf("Module name: %s\nModule code: %s%d\nModule major: %s\nPoint Carried: %d", modules[i].module.name, modules[i].module.post, modules[i].module.code, modules[i].major, modules[i].point);
						printf("\n\n");

						location[num] = i;
						num++;

					}

				}

				choice = choice_validation(1, count_module - student[L].module_count + 1);

				if (choice != 1) {

					student[L].module_count++;
					student[L].point += modules[location[choice - 2]].point;

					FILE* fptr;
					fptr = fopen("Student_point_management.txt", "w");

					for (i = 0; i < count_student; i++) {

						fprintf(fptr, "%s$%s$%d$%d$%d$%d", student[i].stdt_data.name, student[i].stdt_data.post, student[i].stdt_data.code, student[i].point, student[i].module_count, student[i].event_count);

						if (student[i].stdt_data.code == id & student[L].module_count == 1) {
							fprintf(fptr, "$%s&%s&%d", modules[location[choice - 2]].module.name, modules[location[choice - 2]].module.post, modules[location[choice - 2]].module.code);
						}

						else if (student[i].stdt_data.code != id) {

							for (j = 0; j < student[i].module_count; j++)
								fprintf(fptr, "$%s&%s&%d", student[i].module_dn[j].name, student[i].module_dn[j].post, student[i].module_dn[j].code);

						}

						else {

							if (student[i].stdt_data.code == id && modules[location[choice - 2]].module.code < student[i].module_dn[0].code)
								fprintf(fptr, "$%s&%s&%d", modules[location[choice - 2]].module.name, modules[location[choice - 2]].module.post, modules[location[choice - 2]].module.code);

							for (j = 0; j < student[i].module_count - 1; j++) {
								fprintf(fptr, "$%s&%s&%d", student[i].module_dn[j].name, student[i].module_dn[j].post, student[i].module_dn[j].code);

								if (student[i].stdt_data.code == id && student[i].module_dn[j].code < modules[location[choice - 2]].module.code && modules[location[choice - 2]].module.code < student[i].module_dn[j + 1].code)
									fprintf(fptr, "$%s&%s&%d", modules[location[choice - 2]].module.name, modules[location[choice - 2]].module.post, modules[location[choice - 2]].module.code);
							}

							if (student[i].stdt_data.code == id && student[i].module_dn[student[i].module_count - 2].code < modules[location[choice - 2]].module.code)
								fprintf(fptr, "$%s&%s&%d", modules[location[choice - 2]].module.name, modules[location[choice - 2]].module.post, modules[location[choice - 2]].module.code);
						}

						for (j = 0; j < student[i].event_count; j++)
							fprintf(fptr, "$%s&%s&%d", student[i].evnt_dn[j].name, student[i].evnt_dn[j].post, student[i].evnt_dn[j].code);

						fprintf(fptr, "\n");

					}

					fclose(fptr);

				}

				free(location);

			}

			else
				printf("   *All module has been choose*\n\n");

		}

		else if (choice == 3) {

			int index = 0;
			int num = 0;

			if (student[L].event_count != count_event) {

				int* location;
				location = (int*)calloc(count_event - student[L].event_count, sizeof(int));

				printf("\nEvent List\n\n1. Return to Management Activities\n\n");

				for (i = 0; i < count_event; i++) {

					if (student[L].event_count != 0) {

						if (student[L].evnt_dn[index].code != firstptr[i].event.code) {

							printf("%d.\n", num + 2);
							printf("Event name: %s\nEvent code: %s%d\n", firstptr[i].event.name, firstptr[i].event.post, firstptr[i].event.code);
							printf("Point Carried: %d\n", firstptr[i].point);
							printf("Program Leader: %s\nPL code: %s%d\n", firstptr[i].prs_in_chrg.name, firstptr[i].prs_in_chrg.post, firstptr[i].prs_in_chrg.code);
							printf("%d // %d // %d to %d // %d // %d\n", firstptr[i].date[0].day, firstptr[i].date[0].month, firstptr[i].date[0].year, firstptr[i].date[1].day, firstptr[i].date[1].month, firstptr[i].date[1].year);
							printf("Event type: %s\n", firstptr[i].type);
							printf("Category: ");

							for (int j = 0; j < (get_num_of_cat(count_category, count_event, i)); j++) {
								printf("%s  ", firstptr[i].category[j]);
							}

							printf("\n\n");

							location[num] = i;
							num++;

						}

						else
							index++;
					}

					else {

						printf("%d.\n", num + 2);
						printf("Event name: %s\nEvent code: %s%d\n", firstptr[i].event.name, firstptr[i].event.post, firstptr[i].event.code);
						printf("Point Carried: %d\n", firstptr[i].point);
						printf("Program Leader: %s\nPL code: %s%d\n", firstptr[i].prs_in_chrg.name, firstptr[i].prs_in_chrg.post, firstptr[i].prs_in_chrg.code);
						printf("%d // %d // %d to %d // %d // %d\n", firstptr[i].date[0].day, firstptr[i].date[0].month, firstptr[i].date[0].year, firstptr[i].date[1].day, firstptr[i].date[1].month, firstptr[i].date[1].year);
						printf("Event type: %s\n", firstptr[i].type);
						printf("Category: ");

						for (int j = 0; j < (get_num_of_cat(count_category, count_event, i)); j++) {
							printf("%s  ", firstptr[i].category[j]);
						}

						printf("\n\n");

						location[num] = i;
						num++;

					}
				}

				choice = choice_validation(1, count_event - student[L].event_count + 1);

				if (choice != 1) {

					student[L].event_count++;
					student[L].point += firstptr[location[choice - 2]].point;

					FILE* fptr;
					fptr = fopen("Student_point_management.txt", "w");

					for (i = 0; i < count_student; i++) {
						fprintf(fptr, "%s$%s$%d$%d$%d$%d", student[i].stdt_data.name, student[i].stdt_data.post, student[i].stdt_data.code, student[i].point, student[i].module_count, student[i].event_count);

						for (j = 0; j < student[i].module_count; j++)
							fprintf(fptr, "$%s&%s&%d", student[i].module_dn[j].name, student[i].module_dn[j].post, student[i].module_dn[j].code);


						if (student[i].stdt_data.code == id && student[i].event_count == 1) {
							fprintf(fptr, "$%s&%s&%d", firstptr[location[choice - 2]].event.name, firstptr[location[choice - 2]].event.post, firstptr[location[choice - 2]].event.code);
						}

						else if (student[i].stdt_data.code != id) {

							for (j = 0; j < student[i].event_count; j++)
								fprintf(fptr, "$%s&%s&%d", student[i].evnt_dn[j].name, student[i].evnt_dn[j].post, student[i].evnt_dn[j].code);

						}

						else {
							if (student[i].stdt_data.code == id && firstptr[location[choice - 2]].event.code < student[i].evnt_dn[0].code)
								fprintf(fptr, "$%s&%s&%d", firstptr[location[choice - 2]].event.name, firstptr[location[choice - 2]].event.post, firstptr[location[choice - 2]].event.code);

							for (j = 0; j < student[i].event_count - 1; j++) {
								fprintf(fptr, "$%s&%s&%d", student[i].evnt_dn[j].name, student[i].evnt_dn[j].post, student[i].evnt_dn[j].code);

								if (student[i].stdt_data.code == id && student[i].evnt_dn[j].code < firstptr[location[choice - 2]].event.code && firstptr[location[choice - 2]].event.code < student[i].evnt_dn[j + 1].code)
									fprintf(fptr, "$%s&%s&%d", firstptr[location[choice - 2]].event.name, firstptr[location[choice - 2]].event.post, firstptr[location[choice - 2]].event.code);
							}

							if (student[i].stdt_data.code == id && student[i].evnt_dn[student[i].event_count - 2].code < firstptr[location[choice - 2]].event.code)
								fprintf(fptr, "$%s&%s&%d", firstptr[location[choice - 2]].event.name, firstptr[location[choice - 2]].event.post, firstptr[location[choice - 2]].event.code);

						}

						fprintf(fptr, "\n");

					}
					fclose(fptr);

				}
				free(location);
			}

			else
				printf("   *All event have been register*\n\n");

		}

	}
	else {
		printf("   *Read User Error*");
		exit(0);
	}

	for (i = 0; i < count_student; i++) {

		if (student[i].module_count > 0)
			free(student[i].module_dn);

		if (student[i].event_count > 0)
			free(student[i].evnt_dn);
	}

	free(student);

	for (int fr_evn = 0; fr_evn < count_event; fr_evn++)
		for (int fre_cat = 0; fre_cat < (get_num_of_cat(count_category, count_event, fr_evn)); fre_cat++)
			free(firstptr[fr_evn].category[fre_cat]);

	free(firstptr);

	free(modules);

}

void unregister_student_activities(int id) {

	int i, j, choice;
	int index;

	int count_student = 0;
	point_mngmt* student = NULL;
	read_Student_point(&count_student, &student);

	int count_event = 0;
	int count_category = 0;
	activities* firstptr = NULL;
	read_Data_activ(&count_event, &firstptr, &count_category);

	int count_module = 0;
	module* modules = NULL;
	read_Data_reqt(&count_module, &modules);

	int L, R, mid;
	int L1, R1, mid1, key;
	L = -1;
	R = count_student;

	while ((L + 1) != R) {
		mid = (L + R) / 2;
		if (student[mid].stdt_data.code <= id)
			L = mid;
		else
			R = mid;
	}
	if (L >= 0 && student[L].stdt_data.code == id) {

		if (student[L].event_count > 0 || student[L].module_count > 0) {

			int num = 2;
			printf("\nStudent Activities\n\n1. Back\n\n");

			if (student[L].module_count > 0) {

				printf("   Module\n\n");

				for (i = 0; i < student[L].module_count; i++) {

					L1 = -1;
					R1 = count_module;

					while ((L1 + 1) != R1) {
						mid1 = (L1 + R1) / 2;
						if (modules[mid1].module.code <= student[L].module_dn[i].code)
							L1 = mid1;
						else
							R1 = mid1;
					}
					if (L1 >= 0 && modules[L1].module.code == student[L].module_dn[i].code) {

						printf("%d.\n", num);
						printf("Module name: %s\nModule code: %s%d\nModule major: %s\nPoint Carried: %d", modules[L1].module.name, modules[L1].module.post, modules[L1].module.code, modules[L1].major, modules[L1].point);
						printf("\n\n");

						num++;
					}
					else {
						printf("   *Not able to find module details*");
						exit(0);
					}
				}
			}

			if (student[L].event_count > 0) {

				printf("   Event\n\n");

				for (i = 0; i < student[L].event_count; i++) {

					L1 = -1;
					R1 = count_event;

					while ((L1 + 1) != R1) {
						mid1 = (L1 + R1) / 2;
						if (firstptr[mid1].event.code <= student[L].evnt_dn[i].code)
							L1 = mid1;
						else
							R1 = mid1;
					}
					if (L1 >= 0 && firstptr[L1].event.code == student[L].evnt_dn[i].code) {

						printf("%d.\n", num);
						printf("Event name: %s\nEvent code: %s%d\n", firstptr[L1].event.name, firstptr[L1].event.post, firstptr[L1].event.code);
						printf("Point Carried: %d\n", firstptr[L1].point);
						printf("Program Leader: %s\nPL code: %s%d\n", firstptr[L1].prs_in_chrg.name, firstptr[L1].prs_in_chrg.post, firstptr[L1].prs_in_chrg.code);
						printf("%d // %d // %d to %d // %d // %d\n", firstptr[L1].date[0].day, firstptr[L1].date[0].month, firstptr[L1].date[0].year, firstptr[L1].date[1].day, firstptr[L1].date[1].month, firstptr[L1].date[1].year);
						printf("Event type: %s\n", firstptr[L1].type);
						printf("Category: ");
						for (int j = 0; j < (get_num_of_cat(count_category, count_event, L1)); j++) {
							printf("%s  ", firstptr[L1].category[j]);
						}
						printf("\n\n");

						num++;
					}
					else {
						printf("   *Not able to find event details*");
						exit(0);
					}
				}
			}

			choice = choice_validation(1, student[L].module_count + student[L].event_count + 1);

			if (choice != 1) {

				index = student[L].module_count + 1;

				if (choice <= index) {

					student[L].module_count--;

					L1 = -1;
					R1 = count_module;
					key = student[L].module_dn[choice - 2].code;

					while ((L1 + 1) != R1) {
						mid1 = (L1 + R1) / 2;
						if (modules[mid1].module.code <= key)
							L1 = mid1;
						else
							R1 = mid1;
					}
					if (L1 >= 0 && modules[L1].module.code == key) {
						student[L].point -= modules[L1].point;
					}
					else {
						printf("   *Not able to find module details*");
						exit(0);
					}

				}

				else if (choice > index) {

					student[L].event_count--;

					L1 = -1;
					R1 = count_event;
					key = student[L].evnt_dn[choice - student[L].module_count - 2].code;

					while ((L1 + 1) != R1) {
						mid1 = (L1 + R1) / 2;
						if (firstptr[mid1].event.code <= key)
							L1 = mid1;
						else
							R1 = mid1;
					}
					if (L1 >= 0 && firstptr[L1].event.code == key) {
						student[L].point -= firstptr[L1].point;
					}
					else {
						printf("   *Not able to find event details*");
						exit(0);
					}

				}

				FILE* fptr;
				fptr = fopen("Student_point_management.txt", "w");
				for (i = 0; i < count_student; i++) {

					fprintf(fptr, "%s$%s$%d$%d$%d$%d", student[i].stdt_data.name, student[i].stdt_data.post, student[i].stdt_data.code, student[i].point, student[i].module_count, student[i].event_count);

					if (i == L) {

						if (choice <= index) {

							for (j = 0; j < student[i].module_count + 1; j++) {

								if (j != choice - 2)
									fprintf(fptr, "$%s&%s&%d", student[i].module_dn[j].name, student[i].module_dn[j].post, student[i].module_dn[j].code);

							}

						}

						else {

							for (j = 0; j < student[i].module_count; j++) {

								fprintf(fptr, "$%s&%s&%d", student[i].module_dn[j].name, student[i].module_dn[j].post, student[i].module_dn[j].code);

							}

						}

						if (choice > index) {

							for (j = 0; j < student[i].event_count + 1; j++) {

								if (j != choice - student[L].module_count - 2)
									fprintf(fptr, "$%s&%s&%d", student[i].evnt_dn[j].name, student[i].evnt_dn[j].post, student[i].evnt_dn[j].code);

							}

						}

						else {

							for (j = 0; j < student[i].event_count; j++) {

								fprintf(fptr, "$%s&%s&%d", student[i].evnt_dn[j].name, student[i].evnt_dn[j].post, student[i].evnt_dn[j].code);

							}

						}



					}

					else {

						for (j = 0; j < student[i].module_count; j++)
							fprintf(fptr, "$%s&%s&%d", student[i].module_dn[j].name, student[i].module_dn[j].post, student[i].module_dn[j].code);

						for (j = 0; j < student[i].event_count; j++)
							fprintf(fptr, "$%s&%s&%d", student[i].evnt_dn[j].name, student[i].evnt_dn[j].post, student[i].evnt_dn[j].code);
					}

					fprintf(fptr, "\n");

				}

				fclose(fptr);

			}
		}
		else
			printf("   *No Events/Modules Record*");
	}
	else {
		printf("   *Read User Error*");
		exit(0);
	}

	for (i = 0; i < count_student; i++) {

		if (student[i].module_count - 1 > 0)
			free(student[i].module_dn);

		if (student[i].event_count - 1 > 0)
			free(student[i].evnt_dn);
	}

	free(student);

	for (int fr_evn = 0; fr_evn < count_event; fr_evn++)
		for (int fre_cat = 0; fre_cat < (get_num_of_cat(count_category, count_event, fr_evn)); fre_cat++)
			free(firstptr[fr_evn].category[fre_cat]);

	free(firstptr);

	free(modules);
}

void view_track_activities_credits(int id) {

	int i, j;

	int count_student = 0;
	point_mngmt* student = NULL;
	read_Student_point(&count_student, &student);

	int count_event = 0;
	int count_category = 0;
	activities* firstptr = NULL;
	read_Data_activ(&count_event, &firstptr, &count_category);

	int count_module = 0;
	module* modules = NULL;
	read_Data_reqt(&count_module, &modules);

	int L, R, mid;
	int L1, R1, mid1, key;
	L = -1;
	R = count_student;

	while ((L + 1) != R) {
		mid = (L + R) / 2;
		if (student[mid].stdt_data.code <= id)
			L = mid;
		else
			R = mid;
	}
	if (L >= 0 && student[L].stdt_data.code == id) {

		if (student[L].event_count > 0 || student[L].module_count > 0) {

			int num = 1;
			printf("\n\nStudent Report          %s %s%d\n\n", student[L].stdt_data.name, student[L].stdt_data.post, student[L].stdt_data.code);

			if (student[L].module_count > 0) {

				printf("   Module\n");

				for (i = 0; i < student[L].module_count; i++) {

					L1 = -1;
					R1 = count_module;

					while ((L1 + 1) != R1) {
						mid1 = (L1 + R1) / 2;
						if (modules[mid1].module.code <= student[L].module_dn[i].code)
							L1 = mid1;
						else
							R1 = mid1;
					}
					if (L1 >= 0 && modules[L1].module.code == student[L].module_dn[i].code) {
						printf("%d. %s %s%d      %d\n", num, modules[L1].module.name, modules[L1].module.post, modules[L1].module.code, modules[L1].point);
						num++;
					}
					else {
						printf("   *Not able to find module details*");
						exit(0);
					}
				}

				printf("\n");

			}

			if (student[L].event_count > 0) {

				printf("   Event\n");

				for (i = 0; i < student[L].event_count; i++) {

					L1 = -1;
					R1 = count_event;

					while ((L1 + 1) != R1) {
						mid1 = (L1 + R1) / 2;
						if (firstptr[mid1].event.code <= student[L].evnt_dn[i].code)
							L1 = mid1;
						else
							R1 = mid1;
					}
					if (L1 >= 0 && firstptr[L1].event.code == student[L].evnt_dn[i].code) {
						printf("%d. %s %s%d      %d\n", num, firstptr[L1].event.name, firstptr[L1].event.post, firstptr[L1].event.code, firstptr[L1].point);
						num++;
					}
					else {
						printf("   *Not able to find event details*");
						exit(0);
					}
				}
				printf("\n");
			}

			printf("Total Point: %d", student[L].point);

			int choice;
			printf("\n\nPress 1 to Exit\n");
			choice = choice_validation(1, 1);

		}
		else
			printf("   *No Module/ Event Record*");
	}

	for (i = 0; i < count_student; i++) {

		if (student[i].module_count > 0)
			free(student[i].module_dn);

		if (student[i].event_count > 0)
			free(student[i].evnt_dn);
	}

	free(student);

	for (int fr_evn = 0; fr_evn < count_event; fr_evn++)
		for (int fre_cat = 0; fre_cat < (get_num_of_cat(count_category, count_event, fr_evn)); fre_cat++)
			free(firstptr[fr_evn].category[fre_cat]);

	free(firstptr);

	free(modules);

}

//----------Programme Leader----------//
void Programme_leader(int lg_cd) {
	
	//-----------------------Declaration--------------------------//
	char ctgry[7][BUFFER] = { "Motivational","IT & Technology","Business","Musics","Psychology","Future Trend","Health" };
	char major[8][BUFFER] = { "Engineering","Computer and Technology","Health Professions","Arts & Designs","Philosophy","Foreign Language","Actuarial Science","Finance" };
	char menu[3][BUFFER] = { "List Activity","Analysis","Logout" };
	char event[9][BUFFER] = { "Event","Event Code","Person In Charge","Person In Charge ID","Start Date","End Date","Event Type","Event Category","Back" };
	char code_opt[2][BUFFER] = { "Code","Range" };
	char anlysz[5][BUFFER] = { "Most Interested Category","Most Enrolled Major","Event Period:Point Ratio","Top Contributors","Back" };

	FILE* fptr;
	int count_event = 0, mtched = 0;//
	int count_catgry = 0;
	char keyword[40];
	int inct = 0;//
	char chck_pnt = 'a';
	
	int exted;
	int* pic_cat_lst;
	int evnt_amnt = 0;
	int pic_evnt = 0;
	int pic_cat = 0;
	int cat_lst_nm = 0;
	activities* pic = NULL;
	system_users* lgin = NULL;
	int nm_lst = 0;
	char file_name[50];
	strcpy(file_name, "P_Leader.txt");
	read_user_data(file_name, &nm_lst, &lgin);
	read_Data_activ(&pic_evnt, &pic, &pic_cat);
	int crrt_lgin;
	
	//Define log in info
	for (int lct = 0; lct < nm_lst; lct++) {
		if (lgin[lct].user.code == lg_cd) {
			crrt_lgin = lct;
		}
	}

	pic_cat_lst = (int*)malloc((sizeof(ctgry) / sizeof(ctgry[0])) * sizeof(int));
	for (int evnt = 0; evnt < pic_evnt; evnt++) {
		if (lg_cd == pic[evnt].prs_in_chrg.code) {
			evnt_amnt++;
			for (int ctg_fil = 0; ctg_fil < (get_num_of_cat(pic_cat, pic_evnt, evnt)); ctg_fil++) {
				for (int ctg_lst = 0; ctg_lst < 7; ctg_lst++) {
					if (strcmp((pic[evnt].category[ctg_fil]), (ctgry[ctg_lst])) == 0) {
						exted = 0;
						for (int ext_cat = 0; ext_cat < 7; ext_cat++) {
							if (ctg_lst != pic_cat_lst[ext_cat]) {
								exted++;
							}
							else {
								continue;
							}
						}
						if (exted == 7) {
							pic_cat_lst[cat_lst_nm] = ctg_lst;
							cat_lst_nm++;
						}
					}
				}
			}
		}
	}


	for (int fr_evn = 0; fr_evn < count_event; fr_evn++) {
		for (int fre_cat = 0; fre_cat < (get_num_of_cat(pic_cat, pic_evnt, fr_evn)); fre_cat++) {
			free(pic[fr_evn].category[fre_cat]);
		}
	}
	free(pic);


	//-----------------------Declaration--------------------------//

	printf("\033[4m%-*s%s%*s\033[0m\n\n", 30, "*", "PROGRAM LEADER DESK", 30, "*");
	printf("%-*s%-*s%s\n", 25, "PROGRAM LEADER NAME", 15, ":", lgin[crrt_lgin].user.name);
	printf("%-*s%-*s%s%d\n", 25, "PROGRAM LEADER ID", 15, ":", lgin[crrt_lgin].user.post, lgin[crrt_lgin].user.code);
	printf("%-*s%-*s", 25, "CATEGORY IN CHARGE", 15, ":");
	for (int pl_ct = 0; pl_ct < cat_lst_nm; pl_ct++) {
		printf("%-*s |", 15, ctgry[(pic_cat_lst[pl_ct])]);
	}
	printf("\n");

	printf("%-*s%-*s%d\n\n", 25, "CURRENT ACTIVITY AMOUNT", 15, ":", evnt_amnt);

	for (;;) {
		char choice[3];
		tablelise("Program Leader Menu", menu, 3);
		do {
			printf("Choice: ");
			scanf("%3s", choice);
			//printf("%s.",choice);
		} while ((ch_valid(choice, 1, 3)) == -1);
		rmd("Processing your choice", 2);

		switch (atoi(choice)) {
		case 1:;//---------------------------------List Activity---------------------------------------------------------------------------------
			activities* firstptr = NULL;
			count_catgry = 0;
			count_event = 0;
			mtched = 0;
			read_Data_activ(&count_event, &firstptr, &count_catgry);
			int* search_result_list = NULL;
			for (;;) {
				tablelise("Event Details", event, 9);

				do
				{
					printf("Choice: ");
					scanf("%3s", choice);
				} while (ch_valid(choice, 1, 9) == -1);

				switch (atoi(choice)) {
					// -----------------------------------------String Type Search-----------------------------------------------------------------------//
				case 1:
				case 3:
				case 7:

					printf("\nSearch by keyword: ");
					scanf(" %39[^\n]", keyword);
					mtched = (search_string(&firstptr, keyword, &search_result_list, &count_event, choice));
					break;
					// ---------------------------------------Category Type Search----------------------------------------------------------------------//
				case 8:
					printf("\nSearch by keyword: ");
					scanf(" %39[^\n]", keyword);
					mtched = (search_string_ctgry(&firstptr, keyword, &search_result_list, count_catgry, &count_event, choice));
					break;
					// ------------------------------------------Date Type Search-------------------------------------------------------------------------//
				case 5:
				case 6:; //Declaration immediately after a label
					int day_s, mth_s, yr_s;
					int num_simi;//
					char* date_ent = NULL;
					date_ent = (char*)malloc(12 * sizeof(char));
					//-----------Get Valid Date Input to Search------------//
					if ((strcmp(choice, "5") == 0)) {
						inct = 0;
					}
					else if ((strcmp(choice, "6") == 0)) {
						inct = 1;
					}

					do {
						printf("\nSpecify Date to search >leave 0 for any< (dd/mm/yyyy): ");
						fscanf(stdin, " %s", date_ent);
					} while ((inp_date_vld(date_ent, &day_s, &mth_s, &yr_s)) == -1);

					search_result_list = (int*)malloc(count_event * sizeof(int));
					//---------------------------------Checking date existance--------------------------------------//
					for (int evt = 0; evt < count_event; evt++) {
						int num_simi = 0;
						int itm = firstptr[evt].date[inct].day;
						if (firstptr[evt].date[inct].day == day_s) {
							num_simi++;
						}
						else if (day_s == 0) {
							num_simi++;
						}
						if (firstptr[evt].date[inct].month == mth_s) {
							num_simi++;
						}
						else if (mth_s == 0) {
							num_simi++;
						}
						if (firstptr[evt].date[inct].year == yr_s) {
							num_simi++;
						}
						else if (yr_s == 0) {
							num_simi++;
						}

						if (num_simi == 3) {
							search_result_list[mtched] = evt;
							mtched++;
						}

					}
					search_result_list = (int*)realloc(search_result_list, mtched * sizeof(int));
					free(date_ent);
					break;
					//------------------------------------------Integer Type Search------------------------------------------------------------------------//
				case 2:
				case 4:;
					char typ[3];
					char srch_by[3];
					int cd_srch, lct = 0, start, end;
					mtched = 0;
					if (strcmp(choice, "2") == 0) {
						strcpy(typ, "EV");
					}
					else if (strcmp(choice, "4") == 0) {
						strcpy(typ, "PL");
					}
					tablelise("Search event by:", code_opt, 2);

					do
					{
						printf("Choice: ");
						scanf(" %3s", srch_by);
					} while (ch_valid(srch_by, 1, 2) == -1);
					search_result_list = (int*)malloc(count_event * sizeof(int));

					switch (atoi(srch_by)) {
						//---------------------------------Code Searching--------------------------------------//
					case 1:
						printf("\nEnter 5-digit code: %s", typ);
						scanf(" %d", &cd_srch);

						for (int evt = 0; evt < count_event; evt++) {
							if ((get_Imember(&firstptr, evt, choice)) == cd_srch) {
								search_result_list[mtched] = evt;
								mtched++;
							}
						}
						search_result_list = (int*)realloc(search_result_list, mtched * sizeof(int));
						break;
						//--------------------------------Range Searching--------------------------------------//
					case 2:
						printf("\nEnter 5-digit code from: %s", typ);
						scanf(" %d", &start);
						printf("\nEnter 5-digit code to: %s", typ);
						scanf(" %d", &end);

						for (int evt = 0; evt < count_event; evt++) {
							int cd = (get_Imember(&firstptr, evt, choice));
							if (cd >= start && cd <= end) {
								search_result_list[mtched] = evt;
								mtched++;
							}
						}
						search_result_list = (int*)realloc(search_result_list, mtched * sizeof(int));
						break;
					default:
						break;
					}
				case 9:
					printf("\n");
					break;
				default:
					printf("Somthing Went Wrong");
					break;

				}
				printf("\n");

				//-----------------------------------------------Displaying the Search Results------------------------------------------------------//
				if (strcmp(choice, "9") == 0) {
					printf("\n");
					break;
				}
				else if (mtched == 0) {
					printf("Item not found!\n");
				}
				else {
					printf("( Result Found: %d )\n\n", mtched);
					for (int i = 0; i < mtched; i++) {
						int inct = (search_result_list[i]);
						printf("Event name: %s\nEvent code: %s%d\n", firstptr[inct].event.name, firstptr[inct].event.post, firstptr[inct].event.code);
						printf("Point Carried: %d\n", firstptr[inct].point);
						printf("Program Leader: %s\nPL code: %s%d\n", firstptr[inct].prs_in_chrg.name, firstptr[inct].prs_in_chrg.post, firstptr[inct].prs_in_chrg.code);
						printf("%d // %d // %d to %d // %d // %d\n", firstptr[inct].date[0].day, firstptr[inct].date[0].month, firstptr[inct].date[0].year, firstptr[inct].date[1].day, firstptr[inct].date[1].month, firstptr[inct].date[1].year);
						printf("Event type: %s\n", firstptr[inct].type);
						printf("Category: ");
						for (int j = 0; j < (get_num_of_cat(count_catgry, count_event, inct)); j++) {
							printf("%s  ", firstptr[inct].category[j]);
						}
						printf("\n\n");
					}
				}
				printf("\n-----Press Any Keys to Continue--[x] to back: ");
				scanf(" %c", &chck_pnt);
				if (chck_pnt == 'x' || chck_pnt == 'X') {
					break;
				}
				else {
					chck_pnt = 'a';
					continue;
				}
			}
			//------------------------------------------------------Freeing----------------------------------------------------------------------//

			search_result_list = NULL;
			free(search_result_list);

			for (int fr_evn = 0; fr_evn < count_event; fr_evn++) {
				for (int fre_cat = 0; fre_cat < (get_num_of_cat(count_catgry, count_event, fr_evn)); fre_cat++) {
					firstptr[fr_evn].category[fre_cat] = NULL;
					free(firstptr[fr_evn].category[fre_cat]);
				}
			}
			firstptr = NULL;
			free(firstptr);
			break;


		case 2:; //---------------------------------Analysis---------------------------------------------------------------------------------------
			activities* analys_ptr = NULL;
			count_event = 0;
			count_catgry = 0;
			read_Data_activ(&count_event, &analys_ptr, &count_catgry);

			//Dynamically allocating the memory to double layers
			for (;;) {
				char anlyz_by[3];
				tablelise("Analyze Data", anlysz, 5);
				do {
					printf("Choice: ");
					scanf(" %3s", anlyz_by);
				} while (ch_valid(anlyz_by, 1, 5) == -1);

				switch (atoi(anlyz_by)) {
				case 1:; // Category Distribution

					int accmlt = sum(count_catgry, 0);
					int pnt = 0;
					int** clsify = (int**)malloc(7 * sizeof(int*));
					for (int cat_cls = 0; cat_cls < 7; cat_cls++) {
						clsify[cat_cls] = (int*)malloc(10 * sizeof(int));
					} // 7 categories have 5 items for each where first = amount, second = location of categories, remaining = events location
					// Initialization of arrays
					for (int cat_cls = 0; cat_cls < 7; cat_cls++) {
						clsify[cat_cls][0] = 0;
						clsify[cat_cls][1] = cat_cls;
						for (int evt_lct = 2; evt_lct < 10; evt_lct++) {
							clsify[cat_cls][evt_lct] = -1;
						}
					}
					// {    {0 = Amount,1 = Category,2 = Location......}    }
					for (int evt = 0; evt < count_event; evt++) {
						for (int cat = 0; cat < get_num_of_cat(count_catgry, count_event, evt); cat++) {
							pnt = 0;
							for (int cat_ori = 0; cat_ori < 7; cat_ori++) {
								if ((strcmp(analys_ptr[evt].category[cat], ctgry[cat_ori]) == 0)) {
									clsify[cat_ori][0] = clsify[cat_ori][0] + 1;
									while (clsify[cat_ori][2 + pnt] != -1) {
										pnt++;
									}
									clsify[cat_ori][2 + pnt] = evt;
								}
							}
						}
					}

					srt(&clsify, 7, 0, 0);
					printf("Category Distribution: \n");

					for (int dply = 0; dply < 7; dply++) {
						float prcg = (((float)clsify[dply][0]) / accmlt) * 100;
						printf("\033[4m%*s\033[0m\n\n", 70, "_");
						printf("%*d%-*c%-*s [ %d ] | Percentage of Distribution: %.1f%%\n", 5, dply + 1, 3, '.', 15, ctgry[(clsify[dply][1])], clsify[dply][0], prcg);
						for (int evnts = 2; evnts < ((clsify[dply][0]) + 2); evnts++) {
							printf("%*s%-*s%-s%d\n", 10, "--", 30, analys_ptr[(clsify[dply][evnts])].event.name, analys_ptr[(clsify[dply][evnts])].event.post, analys_ptr[(clsify[dply][evnts])].event.code);
						}
					}
					printf("\033[4m%*s\033[0m\n\n", 70, "_");
					for (int cat_cls = 0; cat_cls < 7; cat_cls++) {
						free(clsify[cat_cls]);
					}
					free(clsify);
					break;
				case 2:; // Most Enrolled Major
					std_dtl* stddetail = NULL;
					system_users* std = NULL;
					int std_ammt = 0;
					int count_std = 0;
					int count_cat = 0;
					read_Data_std_dtl(&count_std, &stddetail, &count_cat);
					char fl[50] = "Student.txt";
					read_user_data(fl, &std_ammt, &std);
					int accmlt_std = sum(count_cat, 0);
					int pnt_std = 0;
					char nm[BUFFER];
					int** clsify_std = (int**)malloc(8 * sizeof(int*));
					for (int cat_cls = 0; cat_cls < 8; cat_cls++) {
						clsify_std[cat_cls] = (int*)malloc(10 * sizeof(int));
					} // 7 categories have 5 items for each where first = amount, second = location of categories, remaining = events location
					// Initialization of arrays
					for (int cat_cls = 0; cat_cls < 8; cat_cls++) {
						clsify_std[cat_cls][0] = 0;
						clsify_std[cat_cls][1] = cat_cls;
						for (int evt_lct = 2; evt_lct < 10; evt_lct++) {
							clsify_std[cat_cls][evt_lct] = -1;
						}
					}
					for (int evt = 0; evt < count_std; evt++) {
						pnt_std = 0;
						for (int cat_ori = 0; cat_ori < 8; cat_ori++) {
							if ((strcmp(stddetail[evt].major, major[cat_ori]) == 0)) {
								clsify_std[cat_ori][0] = clsify_std[cat_ori][0] + 1;
								while (clsify_std[cat_ori][2 + pnt_std] != -1) {
									pnt_std++;
								}
								clsify_std[cat_ori][2 + pnt_std] = evt;
							}
						}
					}
					srt(&clsify_std, 8, 0, 0);
					printf("\033[4m%*s\033[0m\n", 70, "_");
					printf("\033[4m%-*c%-*s| %s%-*d| %s%d|\033[0m\n", 3, "|", 20, "Student Major: ", "Total Student: ", 10, count_std, "Major Available: ", 8);
					for (int mjr = 0; mjr < 8; mjr++) {
						float mjr_prcg = (((float)clsify_std[mjr][0]) / accmlt_std) * 100;
						printf("\033[4m%*s\033[0m\n\n", 70, "_");
						printf("%*d%-*c%-*s [ %d ] | Percentage of Distribution: %.1f%%\n", 5, (mjr + 1), 3, ")", 15, major[(clsify_std[mjr][1])], clsify_std[mjr][0], mjr_prcg);
						for (int std_l = 2; std_l < (clsify_std[mjr][0] + 2); std_l++) {
							for (int nm_dply = 0; nm_dply < std_ammt; nm_dply++) {
								if ((std[nm_dply].user.code) == (stddetail[(clsify_std[mjr][std_l])].std_cd)) {
									printf("%*s%-*s%-s%d\n", 10, "--", 30, std[nm_dply].user.name, "ST", stddetail[(clsify_std[mjr][std_l])].std_cd);
								}
							}
						}
					}
					printf("\033[4m%*s\033[0m\n\n", 70, "_");

					for (int cat_cls = 0; cat_cls < 8; cat_cls++) {
						free(clsify_std[cat_cls]);
					}
					free(clsify_std);
					for (int fr_evn = 0; fr_evn < count_std; fr_evn++) {
						for (int fre_cat = 0; fre_cat < (get_num_of_cat(count_cat, count_std, fr_evn)); fre_cat++) {
							free(stddetail[fr_evn].category[fre_cat]);
						}
					}
					free(stddetail);
					free(std);
					break;
				case 3:; //Event Period:Point Ratio
					activities* evt_ptr = NULL;
					int evt_num = 0;
					int catg_num = 0;

					read_Data_activ(&evt_num, &evt_ptr, &catg_num);
					int* dy_spent_evt = (int*)malloc(evt_num * sizeof(int));

					for (int evt = 0; evt < evt_num; evt++) {
						dy_spent_evt[evt] = days_cal(evt_ptr[evt].date[0].day, evt_ptr[evt].date[0].month, evt_ptr[evt].date[0].year, evt_ptr[evt].date[1].day, evt_ptr[evt].date[1].month, evt_ptr[evt].date[1].year);
					}
					printf(" Event Point Moderator\n");
					printf("\033[4m%*c\033[0m\n", 87, '_');
					printf("\033[4m| %-*s| %-*s| %-*s|\033[0m\n", BUFFER, "Event Name", 20, "Day Spent", 20, "Point Per day");
					for (int evt = 0; evt < evt_num; evt++) {
						float ratio = (float)evt_ptr[evt].point / (float)dy_spent_evt[evt];
						printf("| %-*s| %-*d| %-*.2f|\n", BUFFER, evt_ptr[evt].event.name, 20, dy_spent_evt[evt], 20, ratio);
					}
					printf("\033[4m%-*c%-*c%-*c%c\033[0m\n", 42, '|', 22, '|', 22, '|', '|');



					free(dy_spent_evt);
					for (int fr_evn = 0; fr_evn < count_event; fr_evn++) {
						for (int fre_cat = 0; fre_cat < (get_num_of_cat(catg_num, evt_num, fr_evn)); fre_cat++) {
							free(evt_ptr[fr_evn].category[fre_cat]);
						}
					}
					free(evt_ptr);
					break;
				case 4:;// Top contributors
					point_mngmt* std_pnt = NULL;
					int std_nm = 0;
					read_Student_point(&std_nm, &std_pnt);

					int** std_rk = (int**)malloc(8 * sizeof(int*));
					for (int std_cls = 0; std_cls < (std_nm); std_cls++) {
						std_rk[std_cls] = (int*)malloc(3 * sizeof(int));
					}

					for (int ln = 0; ln < std_nm; ln++) {
						std_rk[ln][0] = std_pnt[ln].point;
						std_rk[ln][1] = ln;
						std_rk[ln][2] = 0;
					}
					srt(&std_rk, std_nm, 0, 2);
					printf(" Top Contribution\n");
					printf("\033[4m%*c\033[0m\n", 87, '_');
					printf("\033[4m| %-*s| %-*s| %-*s|\033[0m\n", BUFFER, "Student Name", 20, "Student ID", 20, "Point Gained");
					for (int lst = 0; lst < std_nm; lst++) {
						printf("| %-*s| %s%-*d| %-*d|\n", BUFFER, std_pnt[(std_rk[lst][1])].stdt_data.name, std_pnt[(std_rk[lst][1])].stdt_data.post, 18, std_pnt[(std_rk[lst][1])].stdt_data.code, 20, std_pnt[(std_rk[lst][1])].point);
					}
					printf("\033[4m%-*c%-*c%-*c%c\033[0m\n", 42, '|', 22, '|', 22, '|', '|');



					for (int std_no = 0; std_no < std_nm; std_no++) {
						if (std_pnt[std_no].module_count > 0)
							free(std_pnt[std_no].module_dn);
						if (std_pnt[std_no].event_count > 0)
							free(std_pnt[std_no].evnt_dn);
					}
					free(std_pnt);
					free(std_rk);
					break;
				case 5:
					break;
				default:
					printf("Somthing Went Wrong");
					break;
				}
				if (strcmp(anlyz_by, "5") == 0) {
					break;
				}
				printf("\n-----Press Any Keys to Continue--[x] to back: ");
				scanf(" %c", &chck_pnt);
				if (chck_pnt == 'x' || chck_pnt == 'X') {
					break;
				}
				else {
					chck_pnt = 'a';
					continue;
				}
			}

			for (int fr_evn = 0; fr_evn < count_event; fr_evn++) {
				for (int fre_cat = 0; fre_cat < (get_num_of_cat(count_catgry, count_event, fr_evn)); fre_cat++) {
					free(analys_ptr[fr_evn].category[fre_cat]);
				}
			}

			free(analys_ptr);

			count_catgry = 0;
			break;
		case 3: //---------------------------------Report-----------------------------------------------------------------------------------------
			printf("\n\n%*s\n\n", BUFFER, "GREAT WORKS AND HAVE A NICE DAY!");
			exit(0);
		}

	}
	free(lgin);
	free(pic_cat_lst);
}

//--------Admin---------//
void Admin() {
	struct users user;
	do {
		int EDD = 0;
		char opt[][40] = { "Register","Edit","Unregister User","Track Login","Logout" };
		tablelise("ADMIN", opt, 5);
		printf("Insert your choice:");
		scanf("%i", &user.input);
		switch (user.input) {
		case(0):
			printf("close program!\n");
			break;
		case(1):
			printf("\nYour choice is to register a new user");
			char input_name[35], user_password[20], user_tpnumber[8], user_input_lvl[3], temp_user_id[100], user_name[35], final_info[100];
			const char user_lvl_type[4][3] = { "ST","PC","PL","AD" };
			printf("\nInsert user type ST(student) PC(Programme Coordinator) PL(Programme Leader) AD(administer):");
			scanf("%s", user_input_lvl); _strupr(user_input_lvl);

			if (strcmp(user_input_lvl, "ST") == 0)
				strcpy(user.lvl, "0") && strcpy(user.file, "Student.txt");
			else if (strcmp(user_input_lvl, "PC") == 0)
				strcpy(user.lvl, "1") && strcpy(user.file, "P_Coordinator.txt");
			else if (strcmp(user_input_lvl, "PL") == 0)
				strcpy(user.lvl, "2") && strcpy(user.file, "P_Leader.txt");
			else if (strcmp(user_input_lvl, "AD") == 0)
				strcpy(user.lvl, "3") && strcpy(user.file, "Admin.txt");
			else {
				printf("Insert invalue\n");
				continue;
			}
			do {
				printf("\nInsert your name:");
				scanf(" %49[^\n]", user.name);
			} while (nm_Validate(user.name) != 0);

			do {
				printf("\nInset your password:"); scanf("%s", user.pass);
				printf("Insert your password twice:"); scanf("%s", user.pass2);
				if (strcmp(user.pass, user.pass2) != 0) {
					printf("|----------Second insert Not Equal to first----------|\n");
					continue;
				}
			} while (pass_validate(user.pass) != 0);
			int txt_line[100];
			int TempID = assignID(atoi(user.lvl), user_input_lvl);

			FILE* fptr2;
			fptr2 = fopen(user.file, "a");
			fprintf(fptr2, "%s$%s$%i$%s\n", user.name, user_input_lvl, TempID, user.pass);
			fclose(fptr2);

			if (strcmp(user.file, "Student.txt") == 0) {
				FILE* fptr3;
				fptr3 = fopen("Student_point_management.txt", "a");
				fprintf(fptr3, "%s$%s$%i$0$0$0\n", user.name, user_input_lvl, TempID);
				fclose(fptr3);
			}

			printf("\nDONE\nReturn to main page\n");
			break;
		case(2):
			strcpy(user.lvl, ""); strcpy(user.ID, "");
			printf("Insert your ID to select(in upper case):");
			scanf(" %s", user.inputID);
			if (GTI(user.inputID) == 0) {//Import Name
				for (int start = 0; start < 2; start++)
					strncat(user.lvl, &user.inputID[start], 1);
				_strupr(user.lvl);
				for (int start = 2; start < 7; start++)
					strncat(user.ID, &user.inputID[start], 1);

				if (strcmp(user.lvl, "ST") == 0)
					strcpy(user.file, "Student.txt");
				else if (strcmp(user.lvl, "PC") == 0)
					strcpy(user.file, "P_Coordinator.txt");
				else if (strcmp(user.lvl, "PL") == 0)
					strcpy(user.file, "P_Leader.txt");
				else if (strcmp(user.lvl, "AD") == 0)
					strcpy(user.file, "Admin.txt");
				else {
					strcpy(user.file, "ERROR");
					continue;
				}

				int count = 0;
				struct EDusers* fuser = NULL;
				read(user.file, &count, &fuser);
				FILE* fptr4 = fopen(user.file, "w");
				for (int i = 0; i < count; i++) {
					if (fuser[i].fID == atoi(user.ID)) {
						do {
							printf("Insert Name(%s):", fuser[i].fname); scanf(" %49[^\n]", user.name);
						} while (nm_Validate(user.name) != 0);
						do {
							printf("Insert Password:"); scanf("%s", user.pass);
						} while (pass_validate(user.pass) != 0);

						if (strcmp(user.file, "Student.txt") == 0) {
							changePMF(atoi(user.ID), user.name);
						}
						fprintf(fptr4, "%s$%s$%i$%s\n", user.name, user.lvl, fuser[i].fID, user.pass);
					}
					else
						fprintf(fptr4, "%s$%s$%i$%s\n", fuser[i].fname, fuser[i].ftype, fuser[i].fID, fuser[i].fpassword);
				}

				fclose(fptr4);
				free(fuser);

			}
			else
				printf("\nYou are insert Name Or PASSWORD\nReturn to main page\n");

			break;
		case(3):
			strcpy(user.lvl, ""); strcpy(user.ID, "");
			printf("Insert your ID to select(in upper case):");
			scanf(" %s", user.inputID);
			if (GTI(user.inputID) == 0) {//Import Name
				for (int start = 0; start < 2; start++)
					strncat(user.lvl, &user.inputID[start], 1);
				_strupr(user.lvl);
				for (int start = 2; start < 7; start++)
					strncat(user.ID, &user.inputID[start], 1);

				if (strcmp(user.lvl, "ST") == 0)
					strcpy(user.file, "Student.txt");
				else if (strcmp(user.lvl, "PC") == 0)
					strcpy(user.file, "P_Coordinator.txt");
				else if (strcmp(user.lvl, "PL") == 0)
					strcpy(user.file, "P_Leader.txt");
				else if (strcmp(user.lvl, "AD") == 0)
					strcpy(user.file, "Admin.txt");
				else {
					strcpy(user.file, "ERROR");
					continue;
				}

				int count = 0;
				struct EDusers* fuser = NULL;
				read(user.file, &count, &fuser);
				FILE* fptr5 = fopen(user.file, "w");
				for (int i = 0; i < count; i++) {
					if (fuser[i].fID == atoi(user.ID)) {
						printf("Insert Password(%s):", fuser[i].fname); scanf("%s", user.pass);
						if (strcmp(fuser[i].fpassword, user.pass) == 0) {
							printf("\nUser deleted\n");
						}
						else {
							printf("\n---PASSWORD ERROR---\nReturn to main page\n");
							fprintf(fptr5, "%s$%s$%i$%s\n", fuser[i].fname, fuser[i].ftype, fuser[i].fID, fuser[i].fpassword);
						}
					}
					else
						fprintf(fptr5, "%s$%s$%i$%s\n", fuser[i].fname, fuser[i].ftype, fuser[i].fID, fuser[i].fpassword);
				}
				fclose(fptr5);
			}
			break;
		case(4):
			track_login();//funtion to tracking user login compare with "login_track located in login page"
			break;
		}
	} while (user.input != 5);
	printf("Program Exit\n\n");
}

//--------Programme Coordinator--------//
int Programme_coordinator() {

	detail* Prg_Leader;
	int count = 0;
	system_users* user = NULL;
	system_users* userptr = NULL;
	module* secondptr = NULL;
	read_Data_module(&count, &secondptr);
	read_user_data("P_Leader.txt", &count, &userptr);
	FILE* fptr;
	int count_event = 0;
	int count_catgry = 0;
	int start = 0;
	int end = 0;
	activities* firstptr = NULL;
	read_Data_activ(&count_event, &firstptr, &count_catgry);
	struct users_2* PL = NULL;
	read("P_Leader.txt", &count, &PL);

	int UI = 0;
	char UI_str[20];
	do {
		char opt[6][40] = { "View Students' courses", "View Events","Adding Activities", "Updating Details","View Activities","Logout" };
		tablelise("Programme Coordinator Menu", opt, 6);
		do {
			printf("Choose your option:");
			scanf(" %d", &UI);
			printf("You selected option: %d\n", UI);
			_itoa(UI, UI_str, 10);   //Convert user input from integer to a string to use in ch_valid function
		} while ((ch_valid(UI_str, 1, 6)) == -1);
		if (UI == 1) {
			char opt[10][40] = { "Engineering", "Computers & Technology", "Health Professions", "Art & Designs", "Philosophy", "Foreign Language", "Actuarial Science", "Finance", "Return","Logout" };
			tablelise("Students' Courses", opt, 10);
			do {
				printf("Please choose the major's core modules you want to look at: ");
				scanf(" %d", &UI);
				printf("You selected option: %d\n", UI);
				_itoa(UI, UI_str, 10);
			} while ((ch_valid(UI_str, 1, 10)) == -1);
			switch (UI) {
			case 1: {
				char opt[6][40] = { "Introduction to Engineering Principles", "Mechanics and Materials", "Thermodynamics and Heat Transfer", "Electrical Circuits and Electronics" ,"Return","Logout" };
				tablelise("Engineering", opt, 6);
				do {
					printf("Please choose option (1-5) to return: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 6)) == -1);
				break;
			}
			case 2: {
				char opt[6][40] = { "Programming Fundamentals", "Data Structures and Algorithms", "Computer Networks", "Database Management Systems", "Return", "Logout" };
				tablelise("Computer and Technology", opt, 6);
				do {
					printf("Please choose option (1-5) to return: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 6)) == -1);
				break;
			}
			case 3: {
				char opt[6][40] = { "Anatomy and Physiology", "Medical Ethics and Professionalism", "Pharmacology", "Health Care Management and Policy", "Return", "Logout" };
				tablelise("Health Professions", opt, 6);
				do {
					printf("Please choose option (1-5) to return: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 6)) == -1);
				break;
			}
			case 4: {
				char opt[6][40] = { "Drawing and Painting Techniques", "Graphic Design and Digital Media", "Health Professions", "Sculpture and 3D Design", "Return", "Logout" };
				tablelise("Art History", opt, 6);
				do {
					printf("Please choose option (1-5) to return: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 6)) == -1);
				break;
			}
			case 5: {
				char opt[6][40] = { "Introduction to Philosophy", "Ethics and Morality", "Epistemology and Metaphysics", "Philosophy of Religion", "Return", "Logout" };
				tablelise("Philosophy", opt, 6);
				do {
					printf("Please choose option (1-5) to return: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 6)) == -1);
				break;
			}
			case 6: {
				char opt[6][40] = { "Introductory Language Course", "Advanced Grammar and Composition", "Conversational Language Skills", "Literature in the Target Language", "Return", "Logout" };
				tablelise("Health Professions", opt, 6);
				do {
					printf("Please choose option (1-5) to return: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 6)) == -1);
				break;
			}
			case 7: {
				char opt[6][40] = { "Probability and Statistics", "Financial Mathematics", "Actuarial Models", "Risk Management and Insurance", "Return", "Logout" };
				tablelise("Actuarial Science", opt, 6);
				do {
					printf("Please choose option (1-5) to return: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 6)) == -1);
				break;
			}
			case 8: {
				char opt[6][40] = { "Investments", "Corporate Finance", "Investment Analysis", "Risk Management", "Return", "Logout" };
				tablelise("Finance", opt, 6);
				do {
					printf("Please choose option (1-5) to return: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 6)) == -1);
				break;
			}
			case 9: {
				printf("Returning to main menu...\n");
				break;
			}
			case 10: {
				printf("Thank you for using Programme Coordinator Menu.\n\n");
				return 1;
			}
			}
		}
		else if (UI == 2) {
			char opt[4][40] = { "Events category", "Events Type", "Return", "Logout" };
			tablelise("Events Menu", opt, 4);
			do {
				printf("Choose your option:");
				scanf(" %d", &UI);
				printf("You selected option %d\n:", UI);
				_itoa(UI, UI_str, 10);
			} while ((ch_valid(UI_str, 1, 4)) == -1);

			//UI = choice_validation(1,4);

			switch (UI) {
			case 1: {
				char opt[9][40] = { "Motivational", "IT and Technology", "Business", "Music", "Psychology", "Future", "Health","Return","Logout" };
				tablelise("Health Professions", opt, 9);
				do {
					printf("Choose your option: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 9)) == -1);
				break;
			}

			case 2: {
				char opt[8][40] = { "Competition", "Seminar", "Talk", "Workshop", "Festival", "Sports", "Return","Logout" };
				tablelise("Health Professions", opt, 8);
				do {
					printf("Choose your option: ");
					scanf(" %d", &UI);
					printf("You selected option: %d\n", UI);
					_itoa(UI, UI_str, 10);
				} while ((ch_valid(UI_str, 1, 8)) == -1);
				break;
			}

			case 3: {
				printf("Returning to main menu...\n");
				break;
			}

			case 4: {
				printf("Thank you for using Programme Coordinator Menu.\n\n");
				return 1;
			}
			}
		}
		else if (UI == 3) {
			char opt[3][40] = { "Adding Activities", "Return", "Logout" };
			tablelise("Adding Activities", opt, 3);
			do {
				printf("Choose your option: ");
				scanf(" %d", &UI);
				printf("You selected option: %d\n", UI);
				_itoa(UI, UI_str, 10);
			} while ((ch_valid(UI_str, 1, 3)) == -1);

			//UI = choice_validation(choice,1,4);

			switch (UI) {
			case 1:
				AddingActivities(&user, &firstptr, UI);
			case 2: 
				printf("Returning to main menu...\n");
				break;
			case 3: 
				printf("Thank you for using Programme Coordinator Menu.\n\n");
				return 1;
			}
		}
		else if (UI == 4) {
			char opt[3][40] = { "Update Details", "Return", "Logout" };
			tablelise("Updating Details", opt, 3);
			do {
				printf("Choose your option: ");
				scanf(" %d", &UI);
				printf("You selected option: %d\n", UI);
				_itoa(UI, UI_str, 10);
			} while ((ch_valid(UI_str, 1, 3)) == -1);

			switch (UI) {
			case 1:
				UpdatingActivities(&firstptr, count_event);
				free(firstptr);
			case 2: {
				printf("Returning to main menu...\n");
				break;
			}
			case 3: {
				printf("Thank you for using Programme Coordinator Menu.\n\n");
				return 1;
			}
			}
		}
		else if (UI == 5) {
			char opt[3][40] = { "View Activities", "Return", "Logout" };
			tablelise("View Activities", opt, 3);
			do {
				printf("Choose your option: ");
				scanf(" %d", &UI);
				printf("You selected option: %d\n", UI);
				_itoa(UI, UI_str, 10);
			} while ((ch_valid(UI_str, 1, 3)) == -1);
			switch (UI) {
			case 1:
				for (int i = 0; i < count_event; i++) {
					printf("Event name: %s\nEvent code: %s%d\n", firstptr[i].event.name, firstptr[i].event.post, firstptr[i].event.code);
					printf("Point Carried: %d\n", firstptr[i].point);
					printf("Program Leader: %s\nPL code: %s%d\n", firstptr[i].prs_in_chrg.name, firstptr[i].prs_in_chrg.post, firstptr[i].prs_in_chrg.code);
					printf("%d // %d // %d to %d // %d // %d\n", firstptr[i].date[0].day, firstptr[i].date[0].month, firstptr[i].date[0].year, firstptr[i].date[1].day, firstptr[i].date[1].month, firstptr[i].date[1].year);
					printf("Event type: %s\n", firstptr[i].type);
					printf("Category: ");
					for (int j = 0; j < (get_num_of_cat(count_catgry, count_event, i)); j++) {
						printf("%s ", firstptr[i].category[j]);
					}
					printf("\n\n");
				}
				printf("\n\n");
				
				break;
			case 2:
				printf("Returning to main menu...\n");
				break;
			case 3:
				printf("Thank you for using Programme Coordinator Menu.\n\n");
				return 1;
			}
		}

	} while (UI != 6);

	for (int fr_evn = 0; fr_evn < count_event; fr_evn++) {
		for (int fre_cat = 0; fre_cat < (get_num_of_cat(count_catgry, count_event, fr_evn)); fre_cat++) {
			free(firstptr[fr_evn].category[fre_cat]);
		}
	}
	free(firstptr);

	if (UI == 6)
		printf("EXIT PROGRAM\n");

	return 1;
}