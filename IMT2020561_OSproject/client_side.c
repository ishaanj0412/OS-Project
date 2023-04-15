#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
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

void loginsingacc(int client_sock);
void loginjointacc(int client_sock);
void loginadmin(int client_sock);
void menu(int client_sock);
void depomoney(int client_sock);
void wdmoney(int client_sock);
void changepwd(int client_sock);
void balanceenq(int client_sock);
void addacc(int client_sock);
void delacc(int client_sock);
void modifyacc(int client_sock);
void srchacc(int client_sock);
void getdetails(int client_sock);

int acctype, currusrid;

void acctypeselect(int client_sock)
{
    while(1)
    {
        write(1, "Choose one of the below options: \n\n", sizeof("Choose one of the below options: \n\n"));

        write(1,"1 : Single User Login\n", sizeof("1 : Single User Login\n"));
		write(1,"2 : Joint-Account User Login\n", sizeof("2 : Joint-Account User Login\n"));
		write(1,"3 : Admin Login\n", sizeof("3 : Admin Login\n"));

        write(1,"Your Selection: ", sizeof("Your Selection"));
        scanf("%d", &acctype);
        printf("Option selected: %d\n", acctype);

        if(acctype == 1)
        {
            loginsingacc(client_sock);
            break;
        }
        else if(acctype == 2)
        {
            loginjointacc(client_sock);
            break;
        }
        else if(acctype = 3)
        {
            loginadmin(client_sock);
            break;
        }
        else
        {
            write(1, "Invalid Option, Please try again\n\n", sizeof("Invalid Option, Please try again\n\n"));
        }
    }
    return;
}

void loginsingacc(int client_sock)
{
    sing_acc curr;
    write(1, "Enter Your UserID: ", sizeof("Enter Your UserID: "));
    scanf("%d", &curr.userID);
    currusrid = curr.userID;
    write(1, "Please enter password: ", sizeof("Please enter password: "));
    scanf("%s", curr.password);

    printf("%d\n", acctype);
    write(client_sock, &acctype, sizeof(int));
    write(client_sock, &curr, sizeof(sing_acc));

    bool res;
    read(client_sock, &res, sizeof(res));

    if(!res)
    {
        write(1, "Invalid Login Credentials!\n\n", sizeof("Invalid Login Credentials!\n\n"));
        acctypeselect(client_sock);
    }
    else
    {
        write(1, "Login Successful!\n\n", sizeof("Login Successful!\n\n"));
    }
    return;
};

void loginjointacc(int client_sock)
{
    jointacc curr;
    write(1, "Enter Your UserID: ", sizeof("Enter Your UserID: "));
    scanf("%d", &curr.userID);
    currusrid = curr.userID;
    write(1, "Please enter password: ", sizeof("Please enter password: "));
    scanf("%s", curr.password);

    write(client_sock, &acctype, sizeof(int));
    write(client_sock, &curr, sizeof(jointacc));

    bool res;
    read(client_sock, &res, sizeof(res));

    if(!res)
    {
        write(1, "Invalid Login Credentials!\n\n", sizeof("Invalid Login Credentials!\n\n"));
        acctypeselect(client_sock);
    }
    else
    {
        write(1, "Login Successful!\n\n", sizeof("Login Successful!\n\n"));
    }
    return;
}

void loginadmin(int client_sock)
{
    admin curr;
    write(1, "Enter Your UserID: ", sizeof("Enter Your UserID: "));
    scanf("%d", &curr.userID);
    currusrid = curr.userID;
    write(1, "Please enter password: ", sizeof("Please enter password: "));
    scanf("%s", curr.password);

    write(client_sock, &acctype, sizeof(int));
    write(client_sock, &curr, sizeof(admin));

    bool res;
    read(client_sock, &res, sizeof(res));

    if(!res)
    {
        write(1, "Invalid Login Credentials!\n\n", sizeof("Invalid Login Credentials!\n\n"));
        acctypeselect(client_sock);
    }
    else
    {
        write(1, "Login Successful!\n\n", sizeof("Login Successful!\n\n"));
    }
    return;
}

void depomoney(int client_sock)
{
    float ammt;
    int task = 1;

    write(1, "Please Enter amount to be deposited: ", sizeof("Please enter amount to be deposited: "));
    scanf("%f", &ammt);

    while(ammt<=0)
    {
        printf("Enter a valid amount!\n");
        write(1, "Please Enter amount to be deposited: ", sizeof("Please enter amount to be deposited: "));
        scanf("%f", &ammt);
    }

    write(client_sock, &task, sizeof(int));
    write(client_sock, &ammt, sizeof(float));

    bool res;
    read(client_sock, &res, sizeof(res));

    if(!res)
    {
        write(1, "Error depositing your money!!\n\n", sizeof("Error depositing your money!!\n\n"));
    }
    else
    {
        write(1, "Money deposit successful!\n\n", sizeof("Money deposit successful!\n"));
    }

    menu(client_sock);
    return;
}

void wdmoney(int client_sock)
{
    float ammt;
    int task = 2;

    write(1, "Please Enter amount to be withdrawn: ", sizeof("Please enter amount to be withdrawn: "));
    scanf("%f", &ammt);

    while(ammt<=0)
    {
        printf("Enter a valid amount!\n");
        write(1, "Please Enter amount to be withdrawn: ", sizeof("Please enter amount to be withdrawn: "));
        scanf("%f", &ammt);
    }

    write(client_sock, &task, sizeof(int));
    write(client_sock, &ammt, sizeof(float));

    bool res;
    read(client_sock, &res, sizeof(res));

    if(!res)
    {
        write(1, "Error withdrawing your money, please check your balance!\n\n", sizeof("Error withdrawing your money, please check your balance!\n\n"));
    }
    else
    {
        write(1, "Money withdraw successful!\n\n", sizeof("Money withdraw successful!\n"));
    }

    menu(client_sock);
    return;
}

void balanceenq(int client_sock)
{
    float ammt;
    int task = 3;
    
    write(client_sock, &task, sizeof(float)); 

    int len = read(client_sock, &ammt, sizeof(float));

    write(1, "Available balance is: Rs.", sizeof("Available balance is: Rs."));
    printf("%0.2f\n\n", ammt);

    menu(client_sock);
    return;
}

void changepwd(int client_sock)
{
    int task = 4;
    char newpass[10];
    
    write(1, "Enter the new password(max 10 characters): ", sizeof("Enter the new password(max 10 characters): "));

    scanf("%s", newpass);

    write(client_sock, &task, sizeof(int));
    write(client_sock, newpass, sizeof(newpass));

    bool res;
    read(client_sock, &res, sizeof(res));

    if(!res)
    {
        write(1, "Error changing your password!\n\n", sizeof("Error changing your password!\n\n"));
    }
    else
    {
        write(1, "Your password has been succesfully updated!\n\n", sizeof("Your password has been succesfully updated!\n\n"));
    }

    menu(client_sock);
    return;
}

void getdetails(int client_sock)
{
	int task = 5;

	write(client_sock, &task, sizeof(int));

	if(acctype == 1)
    {
		sing_acc curr1;
		read(client_sock, &curr1, sizeof(curr1));
		
		printf("User ID : %d\n", curr1.userID);
		printf("Name : %s\n", curr1.name);
		printf("Account Number : %d\n", curr1.account_no);
		printf("Available Balance : Rs.%0.2f\n", curr1.balance);
		printf("Status : %s\n\n", curr1.status);
	}
	else if(acctype == 2)
    {
		jointacc curr2;
		read(client_sock, &curr2, sizeof(curr2));
		
		printf("User ID : %d\n", curr2.userID);
		printf("Name of account's Main Holder: %s\n", curr2.name1);
        printf("Name of account's Secondary Holder: %s\n", curr2.name2);
		printf("Account Number : %d\n", curr2.account_no);
		printf("Available Balance : Rs.%0.2f\n", curr2.balance);
		printf("Status : %s\n\n", curr2.status);
	}

	menu(client_sock);
	return;
}

void addacc(int client_sock)
{
	int task = 1;
	int type;

	write(client_sock, &task, sizeof(int));

	write(1, "Enter the type of account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type of account(1: Normal Account 2: Joint Account) : "));
	scanf("%d", &type);

	write(client_sock, &type, sizeof(int));

	if(type == 1)
    {
		sing_acc add1;
		write(1, "Name of the account holder : ", sizeof("Name of the account holder : "));
		scanf("%s", add1.name);
		write(1, "Password(max 10 characters) : ", sizeof("Password(max 10 characters) : "));
		scanf("%s", add1.password);
		write(1, "Initial Deposit : Rs.", sizeof("Initial Deposit : Rs."));
		scanf("%f", &add1.balance);
		write(client_sock, &add1, sizeof(sing_acc));
	}

	if(type == 2)
    {
		jointacc add2;
		write(1, "Name of the primary account holder : ", sizeof("Name of the primary account holder : "));
		scanf("%s", add2.name1);
		write(1, "Name of the secondary account holder : ", sizeof("Name of the secondary account holder : "));
		scanf("%s", add2.name2);
		write(1, "Password(max 10 characters) : ", sizeof("Password(max 10 characters) : "));
		scanf("%s", add2.password);
		write(1, "Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f", &add2.balance);
		write(client_sock, &add2, sizeof(jointacc));
	}
	
    bool res;
	read(client_sock, &res, sizeof(res));

	if(!res)
    {
		write(1,"Error opening the account!!\n\n",sizeof("Error opening the account!!\n\n"));
	}
	else
    {
		write(1, "Succesfully opened the account!!\n\n", sizeof("Succesfully opened the account!!\n\n"));
	}

	menu(client_sock);
	return;
}

void modifyacc(int client_sock)
{
	int task = 3;
	int type;

	write(client_sock, &task, sizeof(int));

	write(1, "Enter the type of account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type of account(1: Normal Account 2: Joint Account) : "));
	scanf("%d", &type);

	write(client_sock, &type, sizeof(int));

	if(type == 1)
    {
		sing_acc mod1;
        write(1, "Enter User ID : ",sizeof("Enter User ID : "));
		scanf("%d", &mod1.userID);
		write(1, "Enter Account Number : ",sizeof("Enter Account Number : "));
		scanf("%d", &mod1.account_no);
		write(1, "Enter New Name of the account holder : ", sizeof("Enter New Name of the account holder : "));
		scanf("%s", mod1.name);
		write(1, "Password(max 10 characters) : ", sizeof("Password(max 10 characters) : "));
		scanf("%s", mod1.password);
		write(1, "Initial Deposit : Rs.", sizeof("Initial Deposit : Rs."));
		scanf("%f", &mod1.balance);
		write(client_sock, &mod1, sizeof(sing_acc));
	}

	if(type == 2)
    {
		jointacc mod2;
        write(1, "Enter User ID : ",sizeof("Enter User ID : "));
		scanf("%d", &mod2.userID);
		write(1, "Enter Account Number : ",sizeof("Enter Account Number : "));
		scanf("%d", &mod2.account_no);
		write(1, "Enter New Name of the primary account holder : ", sizeof("Enter New Name of the primary account holder : "));
		scanf("%s", mod2.name1);
		write(1, "Enter New Name of the secondary account holder : ", sizeof("Enter New Name of the secondary account holder : "));
		scanf("%s", mod2.name2);
		write(1, "Password(max 10 characters) : ", sizeof("Password(max 10 characters) : "));
		scanf("%s", mod2.password);
		write(1, "Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f", &mod2.balance);
		write(client_sock, &mod2, sizeof(jointacc));
	}
	
    bool res;
	read(client_sock, &res, sizeof(res));

	if(!res)
    {
		write(1,"Error modifying the account!!\n\n",sizeof("Error modifying the account!!\n\n"));
	}
	else
    {
		write(1, "Succesfully modified the account!!\n\n", sizeof("Succesfully modified the account!!\n\n"));
	}

	menu(client_sock);
	return;
}

void delacc(int client_sock)
{
    int task = 2;
    int type, id;

    write(client_sock, &task, sizeof(task));

    write(1, "Enter the type account(1: Normal Account 2: Joint Account): ", sizeof("Enter the type account(1: Normal Account 2: Joint Account): "));
    scanf("%d", &type);

    write(client_sock, &type, sizeof(type));

    write(1, "Enter User ID: ", sizeof("Enter User ID: "));
    scanf("%d", &id);
    write(client_sock, &id, sizeof(id));

    bool res;
    read(client_sock, &res, sizeof(res));

    if(!res)
    {
        write(1, "Error deleting account!\n\n", sizeof("Error deleting account!\n\n"));
    }
    else
    {
        write(1, "Succesfully deleted the account!\n\n", sizeof("Succesfully deleted the account!\n\n"));
    }

    menu(client_sock);
    return;
}

void srchacc(int client_sock)
{
    int task = 4;
    int type, len;

	write(client_sock, &task, sizeof(int));

	write(1, "Enter the type of account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type of account(1: Normal Account 2: Joint Account) : "));
	scanf("%d", &type);

	write(client_sock, &type, sizeof(int));

    if(type == 1)
    {
        sing_acc suser1;
        int userid;
        write(1, "User ID: ", sizeof("User ID: "));
        scanf("%d", &userid);
        write(client_sock, &userid, sizeof(int));

        len = read(client_sock, &suser1, sizeof(sing_acc));

        if(len == 0)
        {
            write(1, "Plese re-check the User ID!\n", sizeof("Plese re-check the User ID!\n"));
        }
        else
        {
            printf("User ID : %d\n", suser1.userID);
			printf("Name : %s\n", suser1.name);
			printf("Account Number : %d\n", suser1.account_no);
			printf("Available Balance : Rs.%0.2f\n", suser1.balance);
			printf("Status : %s\n\n", suser1.status);
        }
    }

    if(type == 2)
    {
        jointacc suser2;
        int userid;
        write(1, "User ID: ", sizeof("User ID: "));
        scanf("%d", &userid);
        write(client_sock, &userid, sizeof(int));

        len = read(client_sock, &suser2, sizeof(jointacc));

        if(len == 0)
        {
            write(1, "Plese re-check the User ID!\n", sizeof("Plese re-check the User ID!\n"));
        }
        else
        {
            printf("User ID : %d\n", suser2.userID);
			printf("Main Account Holder's Name : %s\n", suser2.name1);
            printf("Other Account Holder's Name : %s\n", suser2.name2);
			printf("Account Number : %d\n", suser2.account_no);
			printf("Available Balance : Rs.%0.2f\n", suser2.balance);
			printf("Status : %s\n\n", suser2.status);
        }
    }

    menu(client_sock);
    return;
}

void menu(int client_sock)
{
    int task;

    if(acctype == 1 || acctype == 2)
    {
        write(1, "Select from the below options\n\n", sizeof("Select from the below options\n\n"));
        write(1,"1 : Deposit\n", sizeof("1 : Deposit\n"));
        write(1,"2 : Withdraw\n", sizeof("2 : Withdraw\n"));
		write(1,"3 : Balance Enquiry\n", sizeof("3 : Balance Enquiry\n"));
		write(1,"4 : Password Change\n", sizeof("4 : Password Change\n"));
		write(1,"5 : View Details\n", sizeof("5 : View Details\n"));
		write(1,"6 : Exit\n", sizeof("6 : Exit\n"));

        write(1,"Choose an option : ", sizeof("Choose an option : "));
		scanf("%d", &task);
		printf("Option : %d\n", task);

        switch(task)
        {
		    case 1 :
			    depomoney(client_sock);
			    break;
		    case 2 :
			    wdmoney(client_sock);
			    break;
		    case 3 :
			    balanceenq(client_sock);
			    break;
		    case 4 :
			    changepwd(client_sock);
			    break;
		    case 5 :
			    getdetails(client_sock);
			    break;
		    case 6 :
			    write(client_sock, &task, sizeof(int));
			    write(1, "Thank you\n", sizeof("Thank you\n"));
			    exit(0);
		    default :
			    write(1, "Invalid option!!\n\n", sizeof("Invalid option!!\n\n"));
			    menu(client_sock);
			    break;
		}
    }
    else if(acctype == 3)
    {
        write(1, "Select from the below options\n\n", sizeof("Select from the below options\n\n"));
        write(1, "1 : Add Account\n", sizeof("1 : Add Account\n"));
		write(1, "2 : Delete Account\n", sizeof("2 : Delete Account\n"));
		write(1, "3 : Modify Account\n", sizeof("3 : Modify Account\n"));
		write(1, "4 : Search Account\n", sizeof("4 : Search Account\n"));
		write(1, "5 : Exit\n", sizeof("6 : Exit\n"));

		write(1, "Choose an option : ", sizeof("Choose an option : "));
		scanf("%d", &task);
		printf("Option : %d\n", task);
		
		switch(task)
        {
		    case 1 :
			    addacc(client_sock);
			    break;
		    case 2 :
			    delacc(client_sock);
			    break;
		    case 3 :
			    modifyacc(client_sock);
			    break;
		    case 4 :
			    srchacc(client_sock);
			    break;
		    case 5 :
			    write(client_sock, &task, sizeof(int));
			    write(1,"Thank you\n", sizeof("Thank you\n"));
			    exit(0);
		    default :
			    write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
			    menu(client_sock);
			    break;
		}
    }
}

int main(){
	struct sockaddr_in server;
	int client_sock, msgLength;
	char buff[50];
	char result;

	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	server.sin_port = htons(5555);
    socklen_t a = sizeof(server);
	connect(client_sock, (struct sockaddr *)&server, a);

	acctypeselect(client_sock);
	menu(client_sock);	

	close(client_sock);

	return 0;
}