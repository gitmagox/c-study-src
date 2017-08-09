#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	uid_t uid = getuid(); //取真实用户id
	uid_t euid = geteuid();//取有效用户id

	gid_t gid = getgid(); //取真实组ID
	gid_t egid = getegid();//取有效组ID
	/*
		int setuid( uid_t uid )
		int seteuid( uid_t euid );
		int setgid( gid_t gid );
		int setegid( gid_t egid );
	 */
	printf("userid is %d, effective userid is :%d\n", uid, euid );
	return 0;
}