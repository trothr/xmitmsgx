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

/* a Sample program to show how to call Voltage Protect Stateless API */
class MessageDemo
  {
    public static void main(String args[]) throws Exception
      {
        System.out.println("Message Handler Demonstration program in Java");

        MessageService MessageService = new MessageService();

        String version = new String("");
        try
          {
            version = MessageService.version();
            System.out.println(version);
          }
        catch (Exception e)
          {
            System.out.println(" ");
            System.out.println("catch for MessageService.version");
            System.out.println(e.getMessage());
          }


//      String[] msgs;
//      String[] msgs = new String[4]; //with size
//      String[] msgs = new String[] { "Apple", "Banana", "Orange" };
//      String[] msgs = { "Apple", "Banana", "Orange" };
//      String[] msgs = { "", "Apple", "Banana", "Orange" };

        String mylib = new String("xmitmsgx");
        String myopt = new String("");
        try
          {
            MessageService.init(mylib,myopt);
          }
        catch (Exception e)
          {
            System.out.println(" ");
            System.out.println("catch for MessageService.init");
            System.out.println(e.getMessage());
          }


        String[] msgdummy = { };
        try
          {
            MessageService.print(201,msgdummy);
          }
        catch (Exception e)
          { System.out.println(" ");
            System.out.println("catch for MessageService.print");
            System.out.println(e.getMessage());
          }




        String[] msgs = { "#1","#2","#3","#4","#5","#6","#7","#8","#9","#10" };
        try
          {
            MessageService.print(119,msgs);
            /* message number 119 only has 9 replaceable tokens so #10 is ignored */
          }
        catch (Exception e)
          {
            System.out.println(" ");
            System.out.println("catch for MessageService.print");
            System.out.println(e.getMessage());
          }

// Method:    init
// Method:    print
// Method:    string
// Method:    quit



//      /* ----- first sample, FPE of SSN, partial ------------------ */
//      clearText = dataToEncrypt_SSN;
//      try
//        {
//          System.out.println(" ");
//          System.out.println("     Sample Cryptid: SSN, trailing four left in the clear");
//          System.out.println(" Original cleartext: " + clearText);

//          cipherText = MessageService.protect(clearText, "SSN");
//          /* FPE encryption is always string */
//          System.out.println("         Ciphertext: " + cipherText);

//          try
//            {
//              decipheredText = MessageService.access(cipherText, "SSN");
//              System.out.println("Recovered cleartext: " + decipheredText);
//            }
//           catch (Exception e)
//            {
//              System.out.println(" ");
//              System.out.println("catch for MessageService.access from Cryptid SSN");
//              System.out.println(e.getMessage());
//            }
//        }
//      catch (Exception e)
//        {
//          System.out.println(" ");
//          System.out.println("catch for MessageService.protect on Cryptid SSN");
//          System.out.println(e.getMessage());
//        }


//      System.out.println(" ");
      }
  }


