//Written by Andrew Beers e_driver 11/26/12

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>
#include "e.h"


static ssize_t e_driver_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos)
{
	//variables
	int len = count; 
	int max=0;			
	int difference;
	int i;
	int diff;
	char* hold;
	char* result;
	
	diff = (int)*ppos;			//get the offset
	difference = count - diff;	//get the number of characters asked for
		
		
	result = MALLOC(difference*sizeof(int));//malloc for the resulting array
		
		// create array to hold the numbers of e, only make a bigger array if you need more digits
	if(count > max){
		hold = MALLOC(count*sizeof(int));
		max = count;
		e(buf,len);					//read in characters
		
		//fill in from new buffer
		for(i=0;i<difference;i++){
			result[i] = buf[diff];
			diff++;
		}
	}
	else{
		//fill in from existing buffer
		for(i=0;i<difference;i++){
			result[i] = hold[diff];
			diff++;
		}
	
	}
	
	//copy result to user
	if (copy_to_user(buf, result, len))
		return -EINVAL;
	
	*ppos += len;	//move ppos the length
	FREE(result);   // free result
	
	return len;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations e_driver_fops = {
	.owner		= THIS_MODULE,
	.read		= e_driver_read,
};

static struct miscdevice e_driver_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/e_driver.
	 */
	"e_driver",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&e_driver_fops
};

static int __init
e_driver_init(void)
{
	int ret;

	/*
	 * Create the "e_driver" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/e_driver device using
	 * the default rules.
	 */
	ret = misc_register(&e_driver_dev);
	if (ret)
		printk(KERN_ERR"Unable to register \"e_driver, world!\" misc device\n");

	return ret;
}

module_init(e_driver_init);

static void __exit
e_driver_exit(void)
{
	misc_deregister(&e_driver_dev);
}

module_exit(e_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valerie Henson <val@nmt.edu>");
MODULE_DESCRIPTION("\"e_driver, world!\" minimal module");
MODULE_VERSION("dev");
