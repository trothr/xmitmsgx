/*
 *
 *        Name: MessageService.java (Java sourced interface class)
 *              Language binding layer for XMITMSGX for Java (JNI)
 *        Date: 2023-06-28 (Wed)
 *
 */

package com.casita.xmitmsgx;

public class MessageService {

  static {
      System.loadLibrary("xmmjava"); // xmmjava.dll (Windows) or libxmmjava.so (Unix)
    }

  public static native String version();
  public static native String init(String lib, String opt) throws Exception;
  public static native String print(int mn, String[] arg) throws Exception;
  public static native String string(int mn, String[] arg) throws Exception;
  public static native String quit(String obj) throws Exception;

  }


