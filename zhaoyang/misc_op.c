
#include "hello_world.h"
#include "kd_imgsensor_define.h"
/*
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/mmc.h>*/
//#define CAMERA_PROC_PATH "/proc/driver/camera_infos"
#define camera_infos_size 128
extern char *saved_command_line;
extern char g_cam_infos[camera_infos_size];

int we_get_camera = 0;
//other file 
char *mtkfb_find_lcm_driver(void);
//int mmc_reinit_oldcard(struct mmc_host *host);

//*********

//*********

int zhaoyang_read_hardwordinfo(char *buf);
int zhaoyang_string_translation(char* string,int loc,int direction);
int zhaoyang_duel_camera_infos(char* string);
int get_camera_info(char* string);

int zhaoyang_read_hardwordinfo(char *buf)
{
	char *lcm_name; 
	char *put_char;
	int put_leng = 0;
	int ret_leng = 0;
	char *camera_info;
	int host_ret = 0;
	//int camera_procfd = -1;
	// test 
	//
	printk_zhaoyang("In function : [%s]\n",__FUNCTION__);
	if(NULL == buf)
	{
		return 0;
	}
	// get info *******************************************************
	// lcm
	lcm_name = mtkfb_find_lcm_driver();
	printk_zhaoyang("GET lcm name : [%s]\n",lcm_name);
	put_leng += strlen(lcm_name);
	printk_zhaoyang("Add put_leng leng : [%d]\n",put_leng);
	//camera 
	camera_info = kmalloc(camera_infos_size * sizeof(char),GFP_KERNEL);
	printk_zhaoyang("alloc camera : [%d]\n",camera_infos_size);
	if(NULL == camera_info)
	{
		printk_zhaoyang_error("camera_info kmalloc error!!\n");
		return -1;
	}
	if(get_camera_info(camera_info))
	{
		printk_zhaoyang_error("get camera_info error!!");
	}
	else
	{
		printk_zhaoyang("add put_leng : [%s]\n",camera_info);
		put_leng += strlen(camera_info);
	}
	//*****************************************************************
	// print and save
	printk_zhaoyang("kmalloc put_char\n");
	put_leng = put_leng + 2;
	put_char = (char*)kmalloc(put_leng*sizeof(char),GFP_KERNEL);
	if(NULL == put_char)
	{
		printk_zhaoyang_error("kmalloc failed!!\n");
		return 0;
	}
	printk_zhaoyang("save info\n");
	// save 
	strcpy(put_char,"\0");
	//strcpy(put_char,show);
	strcat(put_char,lcm_name);
	printk_zhaoyang("put_char lcm_name: [%s]\n",put_char);
	strcat(put_char,camera_info);
	printk_zhaoyang("put_char camera_info: [%s]\n",put_char);
	strcat(put_char,"\n");
	printk_zhaoyang("put_char \\n: [%s]\n",put_char);
	printk_zhaoyang("save in buf\n");
	ret_leng = scnprintf(buf,PAGE_SIZE,"%s",put_char);
	// frea and close 
	kfree(put_char);
	kfree(camera_info);
	return ret_leng;
}

int zhaoyang_string_translation(char* string,int loc,int direction)    // 0< left   0> right  LOC is an array subscript
{
	int i = 0;
	int leng = 0;

	if(NULL == string)
	{
		return -1;
	}

	leng = strlen(string);

	if(loc < 0 || loc > leng)
	{
		return -1;
	}

	if(loc + direction < 0)
	{
		return -1;
	}

	if(0 > direction)   // left shift  
	{
		direction = -direction;
		for(i = loc; i <= leng; i++)
		{
			string[i - direction] = string[i];
		}
	}
	else if(direction > 0)   // right shift
	{
		for(i = leng; i>= loc; i--)
		{
			string[i + direction] = string[i];
		}
	}

	return 0;
}

int zhaoyang_duel_camera_infos(char* string)
{
	int i = 0;
	int length = 0;

	if(NULL == string)
	{
		return -1;
	}

	length = strlen(string);
	if(length <= 0 )
	{
		return -1;
	}
	// add '\n'
	while(i < length)
	{
		if(';' == string[i])
		{
			if(' ' == string[i+1])
			{
				string[i+1] = '\n';
				i++;
			}
		}
		i++;
	}
	// remove first ' '
	if(' ' == string[0])
	{
		string[0] = '\n';
	}
	// 
	return 0;
}

extern ACDK_KD_SENSOR_INIT_FUNCTION_STRUCT kdSensorList[MAX_NUM_OF_SUPPORT_SENSOR+1];


int get_camera_info(char* string)
{ /*
	int i = 0;
	unsigned int feature_para_len = 0;
	unsigned int feature_para = 0;
	PSENSOR_FUNCTION_STRUCT sencor_fun = NULL;

	printk_zhaoyang("In function : %s\n",__FUNCTION__);
	for(i =0 ; i < MAX_NUM_OF_SUPPORT_SENSOR + 1 && NULL != kdSensorList[i].drvname[0]
		 	&& NULL != kdSensorList[i].SensorInit; i++)
	{
		feature_para_len = 0;
		feature_para = 0;
		sencor_fun = NULL;

		printk_zhaoyang("kdSensorList[%d] -> drvname : [%s]\n",i,kdSensorList[i].drvname);
		kdSensorList[i].SensorInit(&sencor_fun);
		if(NULL == sencor_fun)
		{
			printk_zhaoyang_error("get sencor function error\n");
			continue;
		}
		// check id ************************
		printk_zhaoyang("check sensor id\n");
		sencor_fun->SensorFeatureControl(SENSOR_FEATURE_CHECK_SENSOR_ID,
					(unsigned char*)&feature_para,&feature_para_len);
		if(feature_para != kdSensorList[i].SensorId)
		{
			printk_zhaoyang_error("check SensorId error : drvname : [%s]\n",
				kdSensorList[i].drvname);
			printk_zhaoyang_error("feature_para : [%x] id : [%x]\n",
				feature_para, kdSensorList[i].SensorId);
		}
		else
		{
			printk_zhaoyang("check success error : drvname : [%s]\n",
				kdSensorList[i].drvname);
		}
	}
	printk_zhaoyang("Out function : %s\n",__FUNCTION__);
	*/
	
	int ret_leng = scnprintf(we_get_camera,21,"we_get_camera : [%d]\n",we_get_camera);

	return ret_leng;
}