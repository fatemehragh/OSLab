#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/types.h>

struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
};

static LIST_HEAD(head);

void struct_birthday(int count){
 
    INIT_LIST_HEAD(&head);
    int i=0;
    for(i=0;i<count;i++){
        struct  birthday *person = kmalloc(sizeof(struct birthday),GFP_KERNEL);
        person -> year = 1995;
        person -> month = 8;
        person -> day = 2;
        list_add_tail(&person->list,&head);
    }
}    	 
 
int simple_init(void)
{
    printk(KERN_INFO "Loading Module\n");
    struct_birthday(5);
    struct birthday *curr;
    list_for_each_entry(curr,&head,list){ 
        printk(KERN_INFO "add : %d %d %d\n",curr->year,curr->month,curr->day); 
    }
    
    return 0;
}
/* this function is called when the module is removed*/
void simple_exit(void)
{
    printk(KERN_INFO "Removing Module\n");
 
    struct list_head *ptr,*next;
    struct birthday *temp;
    
    list_for_each_safe(ptr,next,&head){
        temp = list_entry(ptr,struct birthday,list); 
        printk(KERN_INFO "remove %d %d %d\n",temp->year,temp->month,temp->day); 
        list_del(ptr);
        kfree(temp);
    }
}
/* Macros for registering module entry and exit points.
*/
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module");
MODULE_AUTHOR("SGG");
