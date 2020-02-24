//gcc -S asd.c for Assembly code
//gcc -c asd.c for machine level code
//gcc -wall -o asd.exe asd.c for all
//HW: Read about exit statements in c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<stdarg.h>
#include<math.h>

/*
https://planetcalc.com/1721/
http://www.let.rug.nl/kleiweg/lev/
*/


int min(int a, int b, int c){
	if(a < b){
		if(a > c)
			return c;
		else
			return a;
	}
	else{
		if(b > c)
			return c;
		else
			return b;
	}
}

int Levenshtein_Distance(char *string1, char *string2){
	int n = strlen(string1), m = strlen(string2);
	int array[m+1][n+1];

	int i = 0, j = 0, k = 0;
	while(*(string1 + i) != '\0') {
		array[j][i] = k;
		k++;
		i++;
	}
	array[j][i] = k;
	i = k = 0;
	while(*(string2 + j) != '\0'){
			array[j][i] = k;
			k++;
			j++;
	}
	array[j][i] = k;
	i = j = 0;
	while(*(string1 + i) != '\0') {
		j = 0;
		while(*(string2 + j) != '\0'){
			if(*(string1 + i) == *(string2 + j))
				array[j+1][i+1] = min(array[j][i+1] + 1, array[j+1][i] + 1, array[j][i]);
			else
				array[j+1][i+1] = min(array[j][i+1] + 1, array[j+1][i] + 1, array[j][i] + 1);
			j++;
		}
		i++;
	}
	return array[m][n];
}


typedef struct clusters
{
	char word[50];
	struct clusters *next;

}clusters;

void insert(char *string, clusters **head){
	clusters* temp, *new_node;
	temp = (clusters*)malloc(sizeof(clusters));
	new_node = (clusters*)malloc(sizeof(clusters));
	if (*head == NULL) { //Condition when linked list is empty
		*head = new_node;
		strcpy(new_node->word, string);
		new_node->next = NULL;
	}
	else { //condition when linked list has atleat one element
		temp = *head;
		while (temp->next != NULL)//By using this loop we can go at the end of linked list
			temp = temp->next;
		temp->next = new_node; //linking new node with the linked list
		strcpy(new_node->word, string);
		new_node->next = NULL;
	}
}

void print (clusters* temp) {
	while (temp != NULL) { //Traversing the Linked List to print all its elements
		printf("%s  ", temp->word);
		temp = temp->next;
	}
}

char* return_rep_word(clusters *temp, int a){
	int i;
	for (i = 0; i < a; i++)
		temp = temp->next;
	return temp->word;
}

clusters** ComputeKMeans(clusters **clust_head, clusters *head, int no_of_models){
	int index, n = 5;
	for(index = 0; index < n; index++){
		unsigned int distance1, distance2, flag = 0, i = 0;
		clusters *temp = head;

		while(temp != NULL){
			flag = 0;
			distance1 = Levenshtein_Distance(temp->word, clust_head[0]->word);
			for (i = 1; i < no_of_models; i++){
				distance2 = Levenshtein_Distance(temp->word, clust_head[i]->word);
				if(distance1 > distance2){
					distance1 = distance2;
					flag = i;
				}
			}
			insert(temp->word, &clust_head[flag]);
			temp = temp->next;
		}

		if(index == n-1)
			return clust_head;

		for(i = 0; i < no_of_models; i++){
			int sum1 = 0, sum2 = 0;
			clusters *temp1 = clust_head[i], *temp2 = clust_head[i];
			char new_rep_string[50];
			strcpy(new_rep_string, clust_head[i]->word);

			while(temp1 != NULL){
				sum1 += Levenshtein_Distance(temp2->word, temp1->word);
				temp1 = temp1->next;
			}
			temp2 = temp2->next;

			while(temp2 != NULL){
				temp1 = clust_head[i];
				sum2 = 0;
				while(temp1 != NULL){
					sum2 += Levenshtein_Distance(temp2->word, temp1->word);
					temp1 = temp1->next;
				}
				if(sum1 > sum2){
					sum1 = sum2;
					strcpy(new_rep_string, temp2->word);
				}
				temp2 = temp2->next;
			}
			while(clust_head[i]->next != NULL){
				temp = clust_head[i]->next;
				free(clust_head[i]);
				clust_head[i] = temp;
			}
			temp = clust_head[i]->next;
			free(clust_head[i]);
			clust_head[i] = temp;
			insert(new_rep_string, &clust_head[i]);
		}
///		printf("\n\n\n");
//		printf("Representative words are::\n");
//		for (i = 0; i < no_of_models; i++)
//			printf("%s\n", clust_head[i]->word);
	}
}

int* ComputeHistogram(clusters** clust_head, char* sms, int no_of_models){
	int column;
	int *Histogram_array = (int *)malloc(no_of_models*sizeof(int));
	for (column = 0; column < no_of_models; column++){
		int sum = 0;
		char sms_word[100];
		int i = 0, j = 0;
		while (sms[i] != '\0'){
			if(sms[i] != ' '){
				sms_word[j] = sms[i];
				j++;
			}
			else{
				if( j == 0 ){
					i++;
					continue;
				}
				sms_word[j] = '\0';
				sum += Levenshtein_Distance(clust_head[column]->word, sms_word);
				j = 0;
			}
			i++;
		}
		sms_word[j] = '\0';
		sum += Levenshtein_Distance(clust_head[column]->word, sms_word);

		*(Histogram_array + column) = sum;
	}
	/*for (column = 0; column < no_of_models; column++)
		printf("%d  ", *(Histogram_array + column));
*/	return Histogram_array;
}

int ComputeL1distance(int train[], int test[], int no_of_models){
	int sum = 0, i;
	for (i = 0; i < no_of_models; i++)
		sum += (train[i]-test[i] > 0)?(train[i] - test[i]):(test[i] - train[i]);
	return sum;
}

int main(){
	
	FILE *fp, *fp1, *fp2;
	fp = fopen("english.txt","r");
	fp1 = fopen("train_temp.txt","w");
	fp2 = fopen("test.txt","w");
	if(fp == NULL ||fp1 == NULL || fp2 == NULL){
		printf("Unsuccesful opening of file\n");
		return -1;
	}
	else{
		char ch;
		unsigned int count = 0;
		while((ch = getc(fp)) != EOF){
			if(count < 41 || count > 583)
				putc(tolower(ch), fp1);
			else
				putc(tolower(ch), fp2);
			if(ch == '\n')
				count++;
		}
	}
	fclose(fp);
	fclose(fp1);
	fclose(fp2);

	fp1 = fopen("train_temp.txt","r");
	fp2 = fopen("train.txt","w");

	if(fp1 == NULL || fp2 == NULL){
		printf("Unsuccesful opening of file\n");
		return -1;
	}
	else{
		char ch;
		while((ch = getc(fp1)) != EOF){
			if(ch == '.' || ch == ',')
				putc(' ', fp2);
			else if(ch == '\n')
				putc(' ', fp2);
			else
				putc(ch, fp2);
		}
	}
	fclose(fp1);
	fclose(fp2);

	fp2 = fopen("train.txt","r");
	if(fp2 == NULL){
		printf("Unsuccesful opening of file\n");
		return -1;
	}	
	char string[50];
	clusters *head = NULL;
	int count = 0;
	while(fscanf(fp2, "%s", string) != EOF){
		if (strlen(string) > 1 && strcmp(string, "ham") != 0 && strcmp(string, "spam") != 0){
			insert(string, &head);
			count++;
		}
	}
	fclose(fp2);
	//print(head);

	printf("Enter no. of clusters you want to make(>20 and <50)::");
	unsigned int n;
	scanf("%u",&n);
	clusters **clust_head = (clusters**)malloc(n*sizeof(clusters*));

	int i;
	for (i = 0; i < n; i++)
		clust_head[i] = NULL;

	int a = rand()%(count/n), b = rand()%(count/n);
	//printf("%d     %d\n",a,b);
	printf("Please Wait....\n");
	for (i = 0; i < n; i++){
		char *rep_word = return_rep_word(head, a + i*b);
		insert(rep_word, &clust_head[i]);
	}
/*	printf("Random representative strings are::\n");
	for (i = 0; i < n; i++)
		printf("%s\t", clust_head[i]->word);
*/
	clust_head = ComputeKMeans(clust_head, head, n);

/*
	for(i = 0; i < n; i++){
		clusters *temp0 = clust_head[i];
		while(temp0 != NULL){
			printf("%s  ", temp0->word);
			temp0 = temp0->next;
		}
		printf("\n\n");
	}
*/
	printf("\nAfter calculating KMeans representative strings become::\n");
	for (i = 0; i < n; i++)
		printf("%s\t", clust_head[i]->word);

	//The function ComputeHistogram() returns an integer array, which is a histogram

    fp1 = fopen("train_temp.txt","r");
    if(fp1 == NULL){
        printf("Could not open file\n");
        return -1;
    }
    char sms[200];
	int *Histogram_train_array[542];
	int j = 0;
	while (fgets(sms, 199, fp1)) {
		//printf("%s", sms);
		//Histogram_array[j] = (int *)malloc((j+1)*sizeof(int));
		Histogram_train_array[j] = ComputeHistogram(clust_head, sms, n);
		//printf("\n");
		j++;
	}
	fclose(fp1);
/*	for(int index1 = 0; index1 < j; index1++){
		for(int index2 = 0; index2 < n; index2++)
			printf("%d\t", Histogram_train_array[index1][index2]);
		printf("\n");
	}*/
	//printf("\n%d\n\n\n\n", j);
	int no_of_train_sms = j;

    fp2 = fopen("test.txt","r");
    if(fp2 == NULL){
        printf("Could not open file\n");
    	return -1;
    }
	int *Histogram_test_array;
	j = 0;
	int ith_sms = 0, right_spam_detection = 0, right_ham_detection = 0;
	while (fgets(sms, 199, fp2)) {
		//printf("%s", sms);
		Histogram_test_array = ComputeHistogram(clust_head, sms, n);
		int diff_1, diff_2;
		diff_1 = ComputeL1distance(Histogram_train_array[i], Histogram_test_array, n);
		for(i = 0; i < no_of_train_sms; i++){
			diff_2 = ComputeL1distance(Histogram_train_array[i], Histogram_test_array, n);
			if(diff_1 > diff_2){
				diff_1 = diff_2;
				ith_sms = i;
			}
		}
		//get ith text message, if ith message is spam then that is also a spam
		
 	   	fp1 = fopen("test.txt","r");
    	if(fp1 == NULL)
        	printf("Could not open file\n");
        char sms_temp[200];
		for(i = 0; i < ith_sms; i++){
			fgets(sms_temp,199,fp1);
		}
		//printf("ith sms :::::::::%s\n", sms_temp);
		if(sms_temp[strlen(sms_temp)-4] == 'p'){
			if(j < 41)
				right_spam_detection++;
		}
		else if(sms_temp[strlen(sms_temp)-4] == 'h'){
			if(j >= 41)
				right_ham_detection++;
		}
		fclose(fp1);

		j++;
	}
	
	printf("\nRight_spam_detected = %d out of 41 spam messages\n",right_spam_detection);
	printf("Right_ham_detected = %d out of 502 ham messages\n", right_ham_detection);
	printf("Accuracy = %f\n", (float)(right_spam_detection+right_ham_detection)/j*100);
	fclose(fp1);
	fclose(fp2);
	/*for(int index1 = 0; index1 < j; index1++){
		for(int index2 = 0; index2 < n; index2++)
			printf("%d\t", Histogram_test_array[index1][index2]);
		printf("\n");
	}*/
	//printf("\n%d\n", j);
	return 0;
}