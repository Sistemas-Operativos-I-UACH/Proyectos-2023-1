#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W. Oliver II");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

#define DEVICE_NAME "superdupermodule"

static int major;
struct cdev char_device;

static int supermodule_open(struct inode*, struct file*);
static int supermodule_release(struct inode*, struct file*);
static ssize_t supermodule_read(struct file*, char*, size_t, loff_t*);
static ssize_t supermodule_write(struct file*, const char*, size_t, loff_t*);
int write_to_user(char *, char *);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = supermodule_open,
    .release = supermodule_release,
    .read = supermodule_read,
    .write = supermodule_write,
};

char user_message[1024];

static int __init lkm_example_init(void) {
    //int result;
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Super Module load failed\n");
        return major;
    }

    printk(KERN_INFO "Super Module has been loaded\n");

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    /*cdev_init(&char_device, &fops);
    char_device.owner = THIS_MODULE;
    // Add the character device to the system
    result = cdev_add(&char_device, major, 1);
    if (result < 0) {
        unregister_chrdev_region(major, 1);
        pr_err("Failed to add the character device\n");
        return result;
    }

    printk(KERN_INFO "The device file for ths module is: /dev/%s\n", DEVICE_NAME);
    */
    return 0;
}

static void __exit lkm_example_exit(void) {
    printk(KERN_INFO "NO ES CIERTO COYONES!!!\n");
}

static int supermodule_open(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Super Module device opened\n");
   return 0;
}


static ssize_t supermodule_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {
    //int errors = 0;
    static char message[256] = {0};
    if (copy_from_user(message, (void *)buffer, len))
        return -EFAULT;
    /*if (len > 1024) {
        printk(KERN_INFO "The message must be 1024 bytes or less");
        return -EFAULT;
    }*/
    strcpy(user_message, message);
    printk(KERN_INFO "From userland: %s\n", (char *)message);
    return len;
}

static int supermodule_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Super Module device closed\n");
   return 0;
}

static ssize_t supermodule_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    printk(KERN_INFO "user_message: %s\n", user_message);
    int message_len = write_to_user(buffer, user_message);
    return message_len == 0 ? message_len : -ENOMEM;
}

int write_to_user(char *buffer, char * message) {
    int message_len = strlen(message);
    return copy_to_user(buffer, message, message_len);
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
