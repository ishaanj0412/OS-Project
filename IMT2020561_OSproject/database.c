#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Single_User_acc
{
	int userID;
	char name[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}sing_acc;

typedef struct Joint_acc
{
	int userID;
	char name1[30];
	char name2[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}jointacc;

typedef struct admin
{
	int userID;
	char username[30];
	char password[10];
}admin;

int getnewsingaccid();
int getnewjointaccid();
int getnewadid();

int getnewsingaccid()
{
    int fd = open("Single_Users", O_RDONLY, 0744);
    sing_acc temp;
    lseek(fd, -sizeof(sing_acc), SEEK_END);
    read(fd, &temp, sizeof(sing_acc));
    close(fd);
    return temp.userID + 1;
};

int getnewjointaccid()
{
    int fd = open("Joint_Users", O_RDONLY, 0744);
    jointacc temp;
    lseek(fd, -sizeof(jointacc), SEEK_END);
    read(fd, &temp, sizeof(jointacc));
    close(fd);
    return temp.userID + 1;
};

int getnewadminid()
{
    int fd = open("Admins", O_RDONLY, 0744);
    admin temp;
    lseek(fd, -sizeof(admin), SEEK_END);
    read(fd, &temp, sizeof(admin));
    close(fd);
    return temp.userID + 1;
};

int main()
{
	int fd = open("Admins", O_RDWR | O_CREAT, 0744);
	int ch = 0;
	admin newad;
	printf("Please enter the name of the admin: ");
	scanf(" %s", newad.username);
	printf("Please enter the password(max 10 characters): ");
	scanf(" %s", newad.password);
	newad.userID = 1000;
	printf("Your userID is : %d\n", newad.userID);
	write(fd, &newad, sizeof(newad));
	printf("Do you want to continue(0/1)? ");
	scanf("%d", &ch);
	while(ch)
	{
		printf("Please enter the name of the admin: ");
		scanf(" %[^\n]", newad.username);
		printf("Please enter the password(max 10 characters): ");
		scanf(" %[^\n]", newad.password);
		newad.userID = getnewadminid();
		printf("Your userID is : %d\n", newad.userID);
		write(fd, &newad, sizeof(admin));
		printf("Do you want to continue(0/1)? ");
		scanf("%d", &ch);
	}
	close(fd);

	fd = open("Single_Users", O_RDWR | O_CREAT, 0744);
	ch = 1;
	sing_acc newsinguser;
	printf("Please enter the name of the normal user: ");
	scanf(" %[^\n]", newsinguser.name);
	printf("Please enter the password(max 10 characters): ");
	scanf(" %[^\n]", newsinguser.password);
	newsinguser.userID = 1000;
	newsinguser.balance = 1000;
	newsinguser.account_no = (newsinguser.userID-1000)+100000;
	printf("Your userID is : %d\n", newsinguser.userID);
	strcpy(newsinguser.status, "ACTIVE");
	write(fd, &newsinguser, sizeof(sing_acc));
	printf("Do you want to continue(0/1)? ");
	scanf("%d", &ch);
	while(ch)
	{
		printf("Please enter the name of the normal user: ");
		scanf(" %[^\n]", newsinguser.name);
		printf("Please enter the password(max 10 characters): ");
		scanf(" %[^\n]", newsinguser.password);
		newsinguser.userID = getnewsingaccid();
		newsinguser.balance = 1000;
		newsinguser.account_no = (newsinguser.userID-1000)+100000;
		printf("Your userID is : %d\n", newsinguser.userID);
		strcpy(newsinguser.status, "ACTIVE");
		write(fd, &newsinguser, sizeof(sing_acc));
		printf("Do you want to continue(0/1)? ");
		scanf("%d", &ch);
	}
	close(fd);

	fd = open("Joint_Users", O_RDWR | O_CREAT, 0744);
	ch = 1;
	jointacc newjoint;
	printf("Please enter the main name of the joint user: ");
	scanf(" %[^\n]", newjoint.name1);
	printf("Please enter the second name of the joint user: ");
	scanf(" %[^\n]", newjoint.name2);
	printf("Please enter the password(max 10 characters): ");
	scanf(" %[^\n]", newjoint.password);
	newjoint.userID = 1000;
	newjoint.balance = 1000;
	newjoint.account_no = (newjoint.userID-1000)+100000;
	printf("Your userID is : %d\n", newjoint.userID);
	strcpy(newjoint.status, "ACTIVE");
	write(fd, &newjoint, sizeof(jointacc));
	printf("Do you want to continue(0/1)? ");
	scanf("%d", &ch);
	while(ch)
	{
		printf("Please enter the main name of the joint user: ");
		scanf(" %[^\n]", newjoint.name1);
		printf("Please enter the second name of the joint user: ");
		scanf(" %[^\n]", newjoint.name2);
		printf("Please enter the password(max 10 characters): ");
		scanf(" %[^\n]", newjoint.password);
		newjoint.userID = getnewjointaccid();
		newjoint.balance = 1000;
		newjoint.account_no = (newjoint.userID-1000)+100000;
		printf("Your userID is : %d\n", newjoint.userID);
		strcpy(newjoint.status, "ACTIVE");
		write(fd, &newjoint, sizeof(jointacc));
		printf("Do you want to continue(0/1)? ");
		scanf("%d", &ch);
	}
	close(fd);
	return 0;
}