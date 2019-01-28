
#include "hello_world.h"

static int zhaoyang_init(void);
static void zhaoyang_exit(void);

static int register_cdev(void);

static loff_t zhaoyang_llseek(struct file* file_p, loff_t offset, int where);
static ssize_t zhaoyang_read(struct file* file_p, char __user* buf, size_t read_size);
static ssize_t zhaoyang_write(struct file* file_p, char __user* buf, size_t read_size);
static int zhaoyang_open(struct inode* i_node, struct file* file_p);
static ssize_t zhaoyang_show(struct device *dev, struct device_attribute *attr,char *buf);
static ssize_t zhaoyang_store(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count);

// misc op
int zhaoyang_read_hardwordinfo(char *buf);
int zhaoyang_string_translation(char* string,int loc,int direction);
int zhaoyang_duel_camera_infos(char* string);
//

static int para = 10;

static int major_device_num = 0;
static int minor_device_num = 0;
static dev_t zhaoyang_dev;

static char *save_read_info = NULL;

static int alloc_state = -1;  // save state  for alloc_chrdev_region
static int cdev_state = -1; // save call state for cdev_alloc

static const struct file_operations zhaoyang_ops = {
	.owner = THIS_MODULE,
	.llseek = zhaoyang_llseek,
	.read = zhaoyang_read,
	.write = zhaoyang_write,
	.open = zhaoyang_open,
};

static struct cdev *zhaoyang_cdev = NULL;
static struct device zhaoyang_devices;

module_init(zhaoyang_init);
module_param(para,int,S_IRUGO);
module_exit(zhaoyang_exit);

MODULE_LICENSE("GPL"); 

//static struct kobject *p_kobj_zy;
//static struct kobject *p_kobj_zy_son;

/*struct attribute {
	const char		*name;
	umode_t			mode;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	bool			ignore_lockdep:1;
	struct lock_class_key	*key;
	struct lock_class_key	skey;
#endif
};
*/
static struct attribute zhaoyang_attr = {
	.name = "zhaoyang_attr",
};
static struct device_attribute zhaoyang_device_attr = {
	.attr = {
		.name = "zhaoyang_attr",
		.mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
	},
	.show = zhaoyang_show,
	.store = zhaoyang_store,
};
/*struct device_attribute {
	struct attribute	attr;
	ssize_t (*show)(struct device *dev, struct device_attribute *attr,
			char *buf);
	ssize_t (*store)(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count);
};*/

//DEVICE_ATTR("zhaoyang_attr",0,zhaoyang_show,zhaoyang_store);

static struct device zhaoyang_device = {
	.kobj = {
		.name = "zhaoyang",
	},
	.init_name = NULL,
	.bus = NULL,
};

static int __init zhaoyang_init(void)
{
	int err = 0;
	printk_zhaoyang(" hello world !\n");
//	p_kobj_zy = kobject_create_and_add("zhaoyang",NULL);
//	p_kobj_zy_son = kobject_create_and_add("zhaoyang_kobj_son",p_kobj_zy);
	printk_zhaoyang("The process is \"%s\" (pid %i)\n",current->comm,current->pid);
	printk_zhaoyang("para = [%d]\n",para);
	printk(KERN_INFO "zhaoyang$$para = [%d]\n",para);
	// get devices number************************************************
	alloc_state = alloc_chrdev_region(&zhaoyang_dev, 0, 1, DEVICES_ZHAOYANG_NAME);
	// check
	if(0 != alloc_state)
	{
		printk_zhaoyang_error("zhaoyang alloc_chrdev_region failed !! \n");
		return 0;
	}
	// register cdev*****************************************************
	register_cdev();
	// print
	major_device_num = MAJOR(zhaoyang_dev);
	minor_device_num = MINOR(zhaoyang_dev);
	
	printk_zhaoyang("major_device_num : [%d], minor_device_num : [%d]\n",
			major_device_num,minor_device_num);
	//creat_file**********************************************************************
	err = device_register(&zhaoyang_device);
	if (err) 
	{
		put_device(&zhaoyang_device);
		return err;
	}
	err = device_create_file(&zhaoyang_device,&zhaoyang_device_attr);
	if(err != 0)
	{
		printk_zhaoyang_error("device_create_file error!!\n");
	}

	return 0;
}

static int register_cdev(void)
{
	// register cdev
	zhaoyang_cdev = cdev_alloc();
	if(NULL == zhaoyang_cdev)
	{
		printk_zhaoyang_error("Call cdev_alloc error!!\n");
		cdev_state = -1;
		return -1;
	}
	printk_zhaoyang("cdev_alloc success!!\n");
	cdev_state = 0;
	// init cdev
	cdev_init(zhaoyang_cdev, &zhaoyang_ops);
	// add in kernel
	printk_zhaoyang("zhaoyang_cdev->kobj.name : [%s]\n",zhaoyang_cdev->kobj.name);
	cdev_state = cdev_add(zhaoyang_cdev,zhaoyang_dev, 1);
	if(cdev_state)
	{
		printk_zhaoyang_error("Call cdev_add error!!\n");
		return -1;
	}
	printk_zhaoyang("cdev_add success!!\n");
	return 0;
}

static void __exit zhaoyang_exit(void)
{
	printk("zhaoyang exit\n");
	if(0 == alloc_state)
	{
		unregister_chrdev_region(zhaoyang_dev, 1);
	}
	// delete cdev
	cdev_del(zhaoyang_cdev);
	//rm file 
	device_remove_file(&zhaoyang_device, &zhaoyang_device_attr);
	// free save_read_info
	if(NULL != save_read_info)
	{
		kfree(save_read_info);
	}
	//
	return ;
}

// file operations **************************************************************

static loff_t zhaoyang_llseek(struct file* file_p, loff_t offset, int where)
{
	printk("zhaoyang(^.^) Now we are in %s\n",__FUNCTION__);
	return 0;
}

static ssize_t zhaoyang_read(struct file* file_p, char __user* buf, size_t read_size)
{
	// copy to user
	printk("zhaoyang(^.^) Now we are in %s\n",__FUNCTION__);
	return 0;
}

static ssize_t zhaoyang_write(struct file* file_p, char __user* buf, size_t read_size)
{
	// copy from user
	printk("zhaoyang(^.^) Now we are in %s\n",__FUNCTION__);
	return 0;
}

static int zhaoyang_open(struct inode* i_node, struct file* file_p)
{
	printk("zhaoyang(^.^) Now we are in %s\n",__FUNCTION__);
	
	i_node -> i_rdev = zhaoyang_dev;
	file_p -> private_data = zhaoyang_dev;

	return 0;
}

static int zhaoyang_release(struct inode *i_node, struct file* file_p)
{
	printk("zhaoyang(^.^) Now we are in %s\n",__FUNCTION__);
	return 0;
}

// *******************************************************************************
extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int *rawdata);

static ssize_t zhaoyang_show(struct device *dev, struct device_attribute *attr,char *buf)
{
	char show[] = "My name is zhaoyang!!\nI want to be a Linux hacker!!\n";
	int ret_leng = 0;
	/*
	printk("zhaoyang_show : %s\n", show);
	
	// if can print
	if(NULL != save_read_info)
	{
		ret_leng = scnprintf(buf,PAGE_SIZE,"%s",save_read_info);
		printk_zhaoyang("ret_leng : [%d]\n",ret_leng);
		return ret_leng;
	}

	ret_leng = zhaoyang_read_hardwordinfo(buf);

	// save in alltimes
	if(NULL == save_read_info)
	{
		save_read_info = kmalloc(strlen(buf)*sizeof(char),GFP_KERNEL);
		if(strlen(buf) != strncpy(save_read_info,buf,strlen(buf)))
		{
			//scnprintf(save_read_info,strlen(buf),"%s",buf)
			printk_zhaoyang_error("Save in save_read_info nubmer failed!!\n");
			printk_zhaoyang_error("strlen(buf) : [%d], strlen(save_read_info):[%d]\n",
				strlen(buf), strlen(save_read_info));
		}
	}

//	ret_leng = scnprintf(buf,strlen(show),"%s",show);
	printk_zhaoyang("ret_leng : [%d]\n",ret_leng);*/

	unsigned int rawdata=0;
	int data[4] = {0, 0, 0, 0};     
	int adc_value=0;
//helitai adc high
	IMM_GetOneChannelValue(1,data,&rawdata);  //AUXADC1, 1.5V is 4096
	adc_value=data[0]*1000+data[1]*10;
	
	ret_leng = scnprintf(buf,PAGE_SIZE,"adb_value : [%d] rawdata : [%d]\n",adc_value,rawdata);

	return ret_leng;
}

static ssize_t zhaoyang_store(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	printk("zhaoyang_store : %s\n",buf);
	return count;
}



