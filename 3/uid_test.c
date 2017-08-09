#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>


//切换到指定用户，用户组，执行
static _Bool switch_to_user( uid_t user_id, gid_t gp_id)
{
	/* 先确何目标用户不是root */
	if ( ( user_id == 0 ) && ( gp_id ==0))
	{
		return false;
	}
	/* 确保当前用户是合法用户：root 或者目标用护 */
	gid_t gid = getgid();
	uid_t uid = getuid();

	if ( ( (gid != 0 ) || ( uid !=0 ) ) && ( (gid != gp_id) || ( uid != user_id ) ) )
	{
		return false;
	}
	/* 如果不是root,则已经是目标用户 */
	if ( uid != 0 )
	{
		return true;
	}

	/* 切换到目标用户 */
	if ( ( setgid( gp_id ) < 0 ) || ( setuid( user_id ) < 0 ) )
	{
		return false;
	}
	return true;
}

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