/*
 *
 *        Name: MessageDemo.java (Java program source)
 *              demonstrate the message handler called from Java
 *        Date: 2023-07-19 (Wed)
 *    See also: MessageDemo.sh, xmmdemoc.c, xmmrexx.rx
 *
 */

import com.casita.xmitmsgx.MessageService;

import java.util.Arrays;

/* a Sample program to show how to call the message handler from Java */
class MessageDemo
  {
    public static void main(String args[]) throws Exception
      {
        System.out.println("Message Handler Demonstration program in Java");

        MessageService MessageService = new MessageService();

        String version = new String("");
        try
          { version = MessageService.version();
            System.out.println(version); }
        catch (Exception e)
          { System.out.println(" ");
            System.out.println("catch for MessageService.version");
            System.out.println(e.getMessage()); }

        String mylib = new String("xmitmsgx");
        String myopt = new String("");
        try
          { MessageService.init(mylib,myopt); }
          /* we are not keeping the service object info at this time  */
        catch (Exception e)
          { System.out.println(" ");
            System.out.println("catch for MessageService.init");
            System.out.println(e.getMessage()); }

        String[] msgdummy = { };
        try
          { MessageService.print(201,msgdummy); }
        catch (Exception e)
          { System.out.println(" ");
            System.out.println("catch for MessageService.print");
            System.out.println(e.getMessage()); }

        String[] msgs = { "#1","#2","#3","#4","#5","#6","#7","#8","#9","#10" };
        try
          { MessageService.print(119,msgs); }
            /* message number 119 only has 9 replaceable tokens so #10 is ignored */
        catch (Exception e)
          { System.out.println(" ");
            System.out.println("catch for MessageService.print");
            System.out.println(e.getMessage()); }

        String mymessage = new String("");
        try
          { mymessage = MessageService.string(119,msgs);
            System.out.println(mymessage + " (string)"); }
        catch (Exception e)
          { System.out.println(" ");
            System.out.println("catch for MessageService.string");
            System.out.println(e.getMessage()); }


        String libobj = new String("");
        try
          { MessageService.quit(libobj); }
        catch (Exception e)
          { System.out.println(" ");
            System.out.println("catch for MessageService.quit");
            System.out.println(e.getMessage()); }

      }
  }


