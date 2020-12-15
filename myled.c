#include <linux/module.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Ryuichi Ueda and Kosuke Habu");
MODULE_DESCRIPTION("driver for 7-SEGMENT LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;

static volatile u32 *gpio_base_a = NULL;
static volatile u32 *gpio_base_b = NULL;
static volatile u32 *gpio_base_c = NULL;
static volatile u32 *gpio_base_d = NULL;
static volatile u32 *gpio_base_e = NULL;
static volatile u32 *gpio_base_f = NULL;
static volatile u32 *gpio_base_g = NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
  int i;
	char c;
	if(copy_from_user (&c, buf, sizeof(char)))
		return -EFAULT;

	if(c == '1') {
  	for(i = 1; i <= 3; i++) {
		  gpio_base_a[7] = 1 << 17;//上
      msleep(500); 
      gpio_base_a[10] = 1 << 17;

      gpio_base_f[7] = 1 << 26;//左上
      msleep(500);
      gpio_base_f[10] = 1 << 26;

      gpio_base_g[7] = 1 << 27;//中
      msleep(500);
      gpio_base_g[10] = 1 << 27;
 
      gpio_base_c[7] = 1 << 23;//右下
      msleep(500);
      gpio_base_c[10] = 1 << 23;

      gpio_base_d[7] = 1 << 24;//下
      msleep(500);
      gpio_base_d[10] = 1 << 24;

      gpio_base_e[7] = 1 << 25;//左下
      msleep(500);
      gpio_base_e[10] = 1 << 25;

      gpio_base_g[7] = 1 << 27;//中
      msleep(500);
      gpio_base_g[10] = 1 << 27;

      gpio_base_b[7] = 1 << 22;//右上
      msleep(500);
      gpio_base_b[10] = 1 << 22;

    }
  }
	return 1;
}

static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	int size = 0;
	char sushi[] = {'s', 'u', 's', 'h', 'i', 0x0A};
	if(copy_to_user(buf+size, (const char *)sushi, sizeof(sushi))){
		printk(KERN_ERR "sushi: copy_to_user failed.\n");
		return -EFAULT;
	}

	size += sizeof(sushi);
	return size;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.read = sushi_read
};

static int __init init_mod(void)
{
	int retval;
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0) {
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}

	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__, MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0) {
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d", MAJOR(dev), MINOR(dev));
	return retval;
	}
	
	cls = class_create(THIS_MODULE, "myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}

	device_create (cls, NULL, dev, NULL, "myled%d", MINOR(dev));
	
	gpio_base_a = ioremap_nocache(0xfe200000, 0xA0);//ラズパイ3の場合は"0x3fe200000"に変更
  gpio_base_b = ioremap_nocache(0xfe200000, 0xA0);
	gpio_base_c = ioremap_nocache(0xfe200000, 0xA0);
  gpio_base_d = ioremap_nocache(0xfe200000, 0xA0);
	gpio_base_e = ioremap_nocache(0xfe200000, 0xA0);
  gpio_base_f = ioremap_nocache(0xfe200000, 0xA0);
	gpio_base_g = ioremap_nocache(0xfe200000, 0xA0);

  const u32 a_7 = 17;
  const u32 b_6 = 22;
  const u32 c_4 = 23;
  const u32 d_2 = 24;
	const u32 e_1 = 25;
  const u32 f_9 = 26;
  const u32 g_10 = 27;

	const u32 index_a = a_7/10;
  const u32 index_b = b_6/10;
	const u32 index_c = c_4/10;
  const u32 index_d = d_2/10;
  const u32 index_e = e_1/10;
  const u32 index_f = f_9/10;
  const u32 index_g = g_10/10;

  const u32 shift_a = (a_7%10)*3;//21
  const u32 shift_b = (b_6%10)*3;//6
  const u32 shift_c = (c_4%10)*3;//9
  const u32 shift_d = (d_2%10)*3;//12
  const u32 shift_e = (e_1%10)*3;//15
  const u32 shift_f = (f_9%10)*3;//18
  const u32 shift_g = (g_10%10)*3;//21

	const u32 mask_a = ~(0x7 << shift_a);//111111111111111000111111111111111
  const u32 mask_b = ~(0x7 << shift_b);
  const u32 mask_c = ~(0x7 << shift_c);
  const u32 mask_d = ~(0x7 << shift_d);
  const u32 mask_e = ~(0x7 << shift_e);
  const u32 mask_f = ~(0x7 << shift_f);
  const u32 mask_g = ~(0x7 << shift_g);

	gpio_base_a[index_a] = (gpio_base_a[index_a] & mask_a) | (0x1 << shift_a);
	gpio_base_b[index_b] = (gpio_base_b[index_b] & mask_b) | (0x1 << shift_b);
	gpio_base_c[index_c] = (gpio_base_c[index_c] & mask_c) | (0x1 << shift_c);
	gpio_base_d[index_d] = (gpio_base_d[index_d] & mask_d) | (0x1 << shift_d);
	gpio_base_e[index_e] = (gpio_base_e[index_e] & mask_e) | (0x1 << shift_e);
	gpio_base_f[index_f] = (gpio_base_f[index_f] & mask_f) | (0x1 << shift_f);
	gpio_base_g[index_g] = (gpio_base_g[index_g] & mask_g) | (0x1 << shift_g);

	return 0;
}

static void __exit cleanup_mod(void) 
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__, MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
