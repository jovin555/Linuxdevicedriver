#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JOVIN");
MODULE_DESCRIPTION("Minimal sysfs demo module");
MODULE_VERSION("0.1");

static int value =0;
static struct kobject *jovin_kobj;

static ssize_t value_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
  return sprintf(buf, "%d\n", value);
}

static ssize_t value_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf, size_t count)
{
  int ret;
  ret = kstrtoint(buf, 10, &value);
  if (ret < 0)
      return ret;
  return count;
}

static struct kobj_attribute value_attribute = 
    __ATTR(value, 0664, value_show,value_store);

static int __init jovin_init(void)
{
    int error = 0;

    jovin_kobj = kobject_create_and_add("jovin_test",kernel_kobj);
    if (!jovin_kobj)
        return -ENOMEM;

    error = sysfs_create_file(jovin_kobj, &value_attribute.attr);
    if(error) {
      pr_err("failed to create sysfs file\n");
      kobject_put(jovin_kobj);
    }

    pr_info("jovin_test module loaded\n");
    return error;
}


static void __exit jovin_exit(void)
{
  kobject_put(jovin_kobj);
  pr_info("jovin_test module unloaded\n");
}


module_init(jovin_init);
module_exit(jovin_exit);

        
    

