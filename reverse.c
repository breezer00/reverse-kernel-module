#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

static int reverse_open(struct inode *inode, struct file *file);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yanlei Zhao <breezer00@gmail.com>");
MODULE_DESCRIPTION("In-kernel phrase reverser");

/* Accept user defined buffer size */
static unsigned long buffer_size = 8192;
module_param(buffer_size, ulong, (S_IRUSR|S_IRGRP|S_IROTH));
MODULE_PARM_DESC(buffer_size, "Internal buffer size");

/* Module entry point */
static struct file_operations reverse_fops = {
  .owner = THIS_MODULE,
  .open = reverse_open,
  .llseek = noop_llseek /* Set our device to be non-seekable */
};

/* miscdevice struct to register a minor number for our device */
static struct miscdevice reverse_misc_device = {
  .minor = MISC_DYNAMIC_MINOR,
  .name = "reverse",
  .fops = &reverse_fops
};

/* Buffer implementation from scratch */
struct buffer {
  char *data, *end, *read_ptr;
  unsigned long size;
};

static struct buffer *buffer_alloc(unsigned long size)
{
  struct buffer *buf;
  buf = kzalloc(sizeof(*buf), GFP_KERNEL);
  if (unlikely(!buf))
    goto out;

  out:
  return buf;
}

/* static int buffer_free(struct buffer *buf) */
/* { */
/*   if(!buf) */
/*     return -1; */
/*   kfree(buf); */
/*   return 0; */
/* } */

static int reverse_open(struct inode *inode, struct file *file)
{
  int err = 0;
  file->private_data = buffer_alloc(buffer_size);
  if(!file->private_data)
    err = -ENOMEM;
  return err;
}
  

static int __init reverse_init(void)
{
  if (!buffer_size)
    return -1;
  misc_register(&reverse_misc_device);
  printk(KERN_INFO
      "reverse device has been registered, buffer size is %lu bytes\n", 
      buffer_size);
  return 0;
}

static void __exit reverse_exit(void)
{
  misc_deregister(&reverse_misc_device);
  printk(KERN_INFO"reverse device has been unregistered\n");
}


module_init(reverse_init);
module_exit(reverse_exit);
