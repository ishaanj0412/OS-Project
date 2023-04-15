#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <pthread.h>

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

sing_acc getsingacc(int id)
{
    int i = id-1000;
	sing_acc curr;
	int fd = open("Single_Users", O_RDONLY,0744);
	
	int fl;
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(sing_acc);
    lck.l_len = sizeof(sing_acc);	             
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(sing_acc), SEEK_SET);
	read(fd, &curr, sizeof(sing_acc));

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return curr;
}

jointacc getjointacc(int id)
{
    int i = id-1000;
	jointacc curr;
	int fd = open("Joint_Users", O_RDONLY, 0744);
	
	int fl;
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(jointacc);
    lck.l_len = sizeof(jointacc);	             
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(jointacc), SEEK_SET);
	read(fd, &curr, sizeof(jointacc));

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return curr;
}

admin getadminacc(int id)
{
    int i = id-1000;
	admin curr;
	int fd=open("Admins", O_RDONLY, 0744);
	
	int fl;
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(admin);
    lck.l_len = sizeof(admin);	             
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(admin), SEEK_SET);
	read(fd, &curr, sizeof(admin));

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return curr;
}

bool checksing_acc(sing_acc curr)
{
	int i = curr.userID-1000;
	int fd = open("Single_Users", O_RDONLY, 0744);
	bool result;
	sing_acc temp;
	
	int fl;
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(sing_acc);
	lck.l_len = sizeof(sing_acc);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(sing_acc), SEEK_SET);
	read(fd, &temp, sizeof(sing_acc));
	if((temp.userID == curr.userID) && !strcmp(temp.password, curr.password) && !strcmp(temp.status, "ACTIVE"))	result = true;
	else result = false;

	printf("result = %d\n", result);
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return result;
}

bool checkjointacc(jointacc curr)
{
	int i = curr.userID-1000;
	int fd = open("Joint_Users", O_RDONLY, 0744);
	bool result;
    jointacc temp;
	
	int fl;
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(jointacc);
	lck.l_len = sizeof(jointacc);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(jointacc), SEEK_SET);
	read(fd, &temp, sizeof(jointacc));
	if((temp.userID == curr.userID) && !strcmp(temp.password, curr.password) && !strcmp(temp.status, "ACTIVE"))	result = true;
	else result = false;

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return result;
}

bool checkadmin(admin curr)
{
	int i = curr.userID-1000;
	int fd = open("Admins", O_RDONLY, 0744);
	bool result;
	admin temp;
	
	int fl;
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(admin);
	lck.l_len = sizeof(admin);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(admin), SEEK_SET);
	read(fd, &temp, sizeof(admin));
	if((temp.userID == curr.userID) && !strcmp(temp.password, curr.password)) result = true;
	else result = false;

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return result;
}

bool deposit(int type, int ID, float ammt)
{
	int i = ID-1000;
	if(type==1)
    {
		int fd = open("Single_Users", O_RDWR, 0744);
		bool result;
		int fl;
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = i*sizeof(sing_acc);    
		lck.l_len = sizeof(sing_acc);	        
		lck.l_pid = getpid();
	
		fl = fcntl(fd, F_SETLKW, &lck);	

		sing_acc curr;
		lseek(fd, i*sizeof(sing_acc), SEEK_SET);
		read(fd, &curr, sizeof(sing_acc));
		
		if(!strcmp(curr.status, "ACTIVE"))
        {
			curr.balance += ammt;
			lseek(fd, sizeof(sing_acc)*(-1), SEEK_CUR);
			write(fd, &curr, sizeof(sing_acc));
			result = true;
		}
		else result = false;

		lck.l_type=F_UNLCK;
		fcntl(fd, F_SETLK, &lck);

		close(fd);
		return result;		
	}
	else if(type==2)
    {
		int fd = open("Joint_Users", O_RDWR, 0744);
		bool result;
		int fl;
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = i*sizeof(jointacc);    
		lck.l_len = sizeof(jointacc);	        
		lck.l_pid = getpid();
	
		fl = fcntl(fd, F_SETLKW, &lck);	

		jointacc curr;
		lseek(fd, i*sizeof(jointacc), SEEK_SET);
		read(fd, &curr, sizeof(jointacc));
		
		if(!strcmp(curr.status, "ACTIVE"))
        {
			curr.balance += ammt;
			lseek(fd, sizeof(jointacc)*(-1), SEEK_CUR);
			write(fd, &curr, sizeof(jointacc));
			result = true;
		}
		else result = false;

		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);

		close(fd);
		return result;		
	}

	return false;
}

bool withdraw(int type, int ID, float ammt)
{
	int i = ID-1000;
	if(type==1)
    {
		int fd = open("Single_Users", O_RDWR, 0744);
		bool result;
		int fl;
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = i*sizeof(sing_acc);    
		lck.l_len = sizeof(sing_acc);	        
		lck.l_pid = getpid();
	
		fl = fcntl(fd, F_SETLKW, &lck);	

		sing_acc curr;
		lseek(fd, i*sizeof(sing_acc), SEEK_SET);
		read(fd, &curr, sizeof(sing_acc));
		
		if(!strcmp(curr.status, "ACTIVE") && curr.balance>=ammt)
        {
			curr.balance -= ammt;
			lseek(fd, sizeof(sing_acc)*(-1), SEEK_CUR);
			write(fd, &curr, sizeof(sing_acc));
			result = true;
		}
		else result = false;

		lck.l_type=F_UNLCK;
		fcntl(fd, F_SETLK, &lck);

		close(fd);
		return result;		
	}
	else if(type==2)
    {
		int fd = open("Joint_Users", O_RDWR, 0744);
		bool result;
		int fl;
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = i*sizeof(jointacc);    
		lck.l_len = sizeof(jointacc);	        
		lck.l_pid = getpid();
	
		fl = fcntl(fd, F_SETLKW, &lck);	

		jointacc curr;
		lseek(fd, i*sizeof(jointacc), SEEK_SET);
		read(fd, &curr, sizeof(jointacc));
		
		if(!strcmp(curr.status, "ACTIVE"))
        {
			curr.balance -= ammt;
			lseek(fd, sizeof(jointacc)*(-1), SEEK_CUR);
			write(fd, &curr, sizeof(jointacc));
			result = true;
		}
		else result = false;

		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);

		close(fd);
		return result;		
	}

	return false;
}

float showbalance(int type, int ID)
{
	int i = ID-1000;
	if(type==1)
    {
		int fd = open("Single_Users", O_RDWR, 0744);
		float bal;
		int fl;
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = i*sizeof(sing_acc);    
		lck.l_len = sizeof(sing_acc);	        
		lck.l_pid = getpid();
	
		fl = fcntl(fd, F_SETLKW, &lck);	

		sing_acc curr;
		lseek(fd, i*sizeof(sing_acc), SEEK_SET);
		read(fd, &curr, sizeof(sing_acc));
		
		if(!strcmp(curr.status, "ACTIVE"))
        {
			bal = curr.balance;
		}
		else bal = 0;

		lck.l_type=F_UNLCK;
		fcntl(fd, F_SETLK, &lck);

		close(fd);
		return bal;		
	}
	else if(type==2)
    {
		int fd = open("Joint_Users", O_RDWR, 0744);
		float bal;
		int fl;
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = i*sizeof(jointacc);    
		lck.l_len = sizeof(jointacc);	        
		lck.l_pid = getpid();
	
		fl = fcntl(fd, F_SETLKW, &lck);	

		jointacc curr;
		lseek(fd, i*sizeof(jointacc), SEEK_SET);
		read(fd, &curr, sizeof(jointacc));
		
		if(!strcmp(curr.status, "ACTIVE"))
        {
			bal = curr.balance;
		}
		else bal = 0;

		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);

		close(fd);
		return bal;		
	}

	return 0;
}

bool changepassword(int type, int ID, char newpass[10])
{
	int i = ID-1000;
	if(type==1)
    {
		int fd = open("Single_Users", O_RDWR, 0744);
		bool result;
		int fl;
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = i*sizeof(sing_acc);    
		lck.l_len = sizeof(sing_acc);	        
		lck.l_pid = getpid();
	
		fl = fcntl(fd, F_SETLKW, &lck);	

		sing_acc curr;
		lseek(fd, i*sizeof(sing_acc), SEEK_SET);
		read(fd, &curr, sizeof(sing_acc));
		
		if(!strcmp(curr.status, "ACTIVE"))
        {
            strcpy(curr.password, newpass);
            lseek(fd, sizeof(sing_acc)*(-1), SEEK_CUR);
			write(fd, &curr, sizeof(sing_acc));
			result = true;
		}
		else result = false;

		lck.l_type=F_UNLCK;
		fcntl(fd, F_SETLK, &lck);

		close(fd);
		return result;		
	}
	else if(type==2)
    {
		int fd=open("Joint_Users", O_RDWR, 0744);
		bool result;
		int fl;
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = i*sizeof(jointacc);    
		lck.l_len = sizeof(jointacc);	        
		lck.l_pid = getpid();
	
		fl = fcntl(fd, F_SETLKW, &lck);	

		jointacc curr;
		lseek(fd, i*sizeof(jointacc), SEEK_SET);
		read(fd, &curr, sizeof(jointacc));
		
		if(!strcmp(curr.status, "ACTIVE"))
        {
			strcpy(curr.password, newpass);
            lseek(fd, sizeof(jointacc)*(-1), SEEK_CUR);
			write(fd, &curr, sizeof(jointacc));
			result = true;
		}
		else result = false;

		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);

		close(fd);
		return result;		
	}

	return false;
}

bool opensingacc(sing_acc newacc)
{
	int fd = open("Single_Users", O_RDWR, 0744);
	bool result;
	
	int fl;
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_END;
	lck.l_start = (-1)*sizeof(sing_acc);
	lck.l_len = sizeof(sing_acc);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);	

	sing_acc temp;
	lseek(fd, (-1)*sizeof(sing_acc), SEEK_END);
	read(fd, &temp, sizeof(sing_acc));
		
	newacc.userID = temp.userID+1;
	newacc.account_no = temp.account_no+1;
	strcpy(newacc.status, "ACTIVE");
	
	int j = write(fd, &newacc, sizeof(newacc));
	if(j!=0) result=true;
	else result=false;
	
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	
	close(fd);
	return result;	
}

bool openjointacc(jointacc newacc)
{
	int fd = open("Joint_Users", O_RDWR, 0744);
	bool result;
	
	int fl;
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_END;
	lck.l_start = (-1)*sizeof(jointacc);
	lck.l_len = sizeof(jointacc);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);	

	jointacc temp;
	lseek(fd, (-1)*sizeof(jointacc), SEEK_END);
	read(fd, &temp, sizeof(jointacc));
		
	newacc.userID = temp.userID+1;
	newacc.account_no = temp.account_no+1;
	strcpy(newacc.status, "ACTIVE");
	
	int j = write(fd, &newacc, sizeof(newacc));
	if(j!=0) result=true;
	else result=false;
	
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	
	close(fd);
	return result;	
}

bool closesingacc(int curr)
{
	int i = curr - 1000;
	int fd = open("Single_Users", O_RDONLY, 0744);
	bool result;
	sing_acc temp;
	
	int fl;
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(sing_acc);
	lck.l_len = sizeof(sing_acc);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(sing_acc), SEEK_SET);
	read(fd, &temp, sizeof(sing_acc));

	if(!strcmp(temp.status, "ACTIVE"))
    {
        strcpy(temp.status, "CLOSED");
		temp.balance = 0;
		lseek(fd, (-1)*sizeof(sing_acc), SEEK_CUR); 
		int j = write(fd, &temp, sizeof(sing_acc));
		if(j!=0) result=true;
		else result=false;
    }

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return result;
}

bool closejointacc(int curr)
{
	int i = curr - 1000;
	int fd = open("Single_Users", O_RDONLY, 0744);
	bool result;
	jointacc temp;
	
	int fl;
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(jointacc);
	lck.l_len = sizeof(jointacc);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(jointacc), SEEK_SET);
	read(fd, &temp, sizeof(jointacc));

	if(!strcmp(temp.status, "ACTIVE"))
    {
        strcpy(temp.status, "CLOSED");
		temp.balance = 0;
		lseek(fd, (-1)*sizeof(jointacc), SEEK_CUR); 
		int j = write(fd, &temp, sizeof(jointacc));
		if(j!=0) result=true;
		else result=false;
    }

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return result;
}

bool modifysingacc(sing_acc curr)
{
	int i = curr.userID - 1000;
	int fd = open("Single_Users", O_RDONLY, 0744);
	bool result;
	sing_acc temp;
	
	int fl;
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(sing_acc);
	lck.l_len = sizeof(sing_acc);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(sing_acc), SEEK_SET);
	read(fd, &temp, sizeof(sing_acc));

	if(!strcmp(temp.status, "ACTIVE") && (curr.account_no == temp.account_no))
    {
        strcpy(curr.status, "ACTIVE");
		lseek(fd, (-1)*sizeof(sing_acc),SEEK_CUR); 
		int j=write(fd, &curr, sizeof(sing_acc));
		if(j!=0) result=true;
		else result=false;
    }

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return result;
}

bool modifyjointacc(jointacc curr)
{
	int i = curr.userID - 1000;
	int fd = open("Single_Users", O_RDONLY, 0744);
	bool result;
	jointacc temp;
	
	int fl;
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = i*sizeof(jointacc);
	lck.l_len = sizeof(jointacc);
	lck.l_pid = getpid();
	
	fl = fcntl(fd, F_SETLKW, &lck);

	lseek(fd, i*sizeof(jointacc), SEEK_SET);
	read(fd, &temp, sizeof(jointacc));

	if(!strcmp(temp.status, "ACTIVE") && (curr.account_no == temp.account_no))
    {
        strcpy(curr.status, "ACTIVE");
		lseek(fd, (-1)*sizeof(jointacc),SEEK_CUR); 
		int j=write(fd, &curr, sizeof(jointacc));
		if(j!=0) result=true;
		else result=false;
    }

	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);

	close(fd);
	return result;
}

void serverTask(int nsd)
{
	int msglen, select, type, option, accType, currUserID;
	bool result;
	while(1)
	{
		read(nsd, &option, sizeof(option));
		printf("Option : %d\n",option);

		if(option == 1)
		{
			sing_acc currUser1;
			accType = 1;
			msglen = read(nsd, &currUser1, sizeof(sing_acc));
			printf("Username : %d\n", currUser1.userID);
			printf("Password : %s\n", currUser1.password);
			currUserID = currUser1.userID;
			result = checksing_acc(currUser1);
			write(nsd, &result, sizeof(result));
		}
		else if(option == 2)
		{
			jointacc currUser2;
			accType = 2;
			msglen = read(nsd,&currUser2,sizeof(jointacc));
			currUserID = currUser2.userID;
			printf("Username : %d\n", currUser2.userID);
			printf("Password : %s\n", currUser2.password);
			result = checkjointacc(currUser2);
			write(nsd, &result, sizeof(result));
		}
		else if(option == 3)
		{
			admin currUser3;
			accType = 3;
			msglen = read(nsd, &currUser3, sizeof(admin));
			currUserID = currUser3.userID;
			printf("Username : %d\n", currUser3.userID);
			printf("Password : %s\n", currUser3.password);
			result = checkadmin(currUser3);
			write(nsd, &result, sizeof(result));
		}
		else
		{
			result = false;
			write(nsd, &result, sizeof(result));
		}
		if(result) break;		
	}

	while(1)
	{
		read(nsd, &select, sizeof(int));
		if(option == 1 || option == 2)
		{
			if(select == 1)
			{
				float amt;
				read(nsd, &amt, sizeof(float));
				result = deposit(accType, currUserID, amt);
				write(nsd, &result, sizeof(result));
			}
			else if(select == 2)
			{
				float amt;
				read(nsd, &amt, sizeof(float));
				result = withdraw(accType, currUserID, amt);
				write(nsd, &result, sizeof(result));
			}
			else if(select == 3)
			{
				float amt;
				amt = showbalance(accType, currUserID);
				write(nsd, &amt, sizeof(float));
			}
			else if(select == 4)
			{
				char pwd[10];
				read(nsd, pwd, sizeof(pwd));
				result = changepassword(accType, currUserID, pwd);
				write(nsd, &result, sizeof(result));
			}
			else if(select == 5)
			{
				if(option==1)
				{
					sing_acc user1 = getsingacc(currUserID);
					write(nsd, &user1, sizeof(sing_acc));
				}
				else if(option == 2)
				{
					jointacc user2 = getjointacc(currUserID);
					write(nsd, &user2, sizeof(jointacc));
				}
			}
			else if(select == 6) break;
		}
		else if(option == 3)
		{
			read(nsd, &type, sizeof(int));
			if(select == 1)
			{
				if(type == 1)
				{
					sing_acc newUser1;
					read(nsd, &newUser1, sizeof(sing_acc));
					result = opensingacc(newUser1);
					write(nsd, &result, sizeof(result));
				}
				else if(type == 2)
				{
					jointacc newUser2;
					read(nsd, &newUser2, sizeof(jointacc));
					result = openjointacc(newUser2);
					write(nsd, &result, sizeof(result));
				}
			}
			else if(select == 2)
			{
				if(type == 1)
				{
					int delUserID1;
					read(nsd, &delUserID1, sizeof(int));
					result = closesingacc(delUserID1);
					write(nsd, &result, sizeof(result));
				}
				else if(type == 2)
				{
					int delUserID2;
					read(nsd, &delUserID2, sizeof(int));
					result = closejointacc(delUserID2);
					write(nsd, &result, sizeof(result));
				}
			}
			else if(select == 3)
			{
				if(type == 1)
				{
					sing_acc modUser1;
					read(nsd, &modUser1, sizeof(sing_acc));
					result = modifysingacc(modUser1);
					write(nsd, &result, sizeof(result));
				}
				else if(type==2)
				{
					jointacc modUser2;
					read(nsd, &modUser2, sizeof(jointacc));
					result = modifyjointacc(modUser2);
					write(nsd, &result, sizeof(result));
				}
			}
			else if(select == 4)
			{
				if(type == 1)
				{
					sing_acc searchUser1;
					int userID1;
					read(nsd, &userID1, sizeof(int));
					searchUser1 = getsingacc(userID1);
					write(nsd, &searchUser1, sizeof(sing_acc));
				}
				else if(type == 2)
				{
					jointacc searchUser2;
					int userID2;
					read(nsd, &userID2, sizeof(int));
					searchUser2 = getjointacc(userID2);
					write(nsd, &searchUser2, sizeof(jointacc));
				}
			}
			else if(select==5) break;
		}
	}
	close(nsd);
	write(1, "Ended client session.....\n",sizeof("Ended client session.....\n"));
	return;
}

void *servertask(void *nsd) 
{
	int nsfd = *(int*)nsd;
	serverTask(nsfd);
}


int main()
{
	struct sockaddr_in server, client;
	int sd, nsd, clientlen;
	pthread_t threads;
	bool result;
	sd = socket(AF_INET, SOCK_STREAM, 0);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5555);

	bind(sd, (struct sockaddr *)&server, sizeof(server));
	listen(sd,5);
	
	write(1, "Waiting for the client.....\n", sizeof("Waiting for the client.....\n"));
	while(1)
	{
		clientlen = sizeof(client);
		nsd = accept(sd,(struct sockaddr *)&client,&clientlen);

		write(1, "Connected to the client.....\n", sizeof("Connected to the client.....\n"));
		if(pthread_create(&threads, NULL, servertask, (void*) &nsd)<0)
		{
			perror("could not create thread");
			return 1;
		}
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}