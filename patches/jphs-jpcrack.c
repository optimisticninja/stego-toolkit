--- jpseek.c
+++ jpcrack.c
@@ -6,12 +6,11 @@
  * Use permitted under terms of GNU Public Licence only.
  *
  */
-
+#define _GNU_SOURCE
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <ctype.h>
-#include <fcntl.h>
 #include <unistd.h>
 #include <getopt.h>
 #include <errno.h>
@@ -20,11 +19,13 @@
 #include <sys/time.h>		
 #include <sys/resource.h>		
 #include <sys/stat.h>		
+#include <fcntl.h>
 #include "bf.h"
 #include "cdjpeg.h"
 #include "version.h"
 
 #define NKSTREAMS 4
+#define PASSLEN 120
 
 static jvirt_barray_ptr * coef_arrays;
 static unsigned int  cpos[NKSTREAMS];
@@ -187,10 +188,10 @@
 
 
 static int jpseek (const char* infilename,
-                   const char* seekfilename)
+		   const char* pass)
 {
+ int result = 0;
  int b,count,i,j,len0,len1,len2;
- char *pass;
  unsigned char iv[9];
  unsigned char v;
  struct jpeg_decompress_struct srcinfo;
@@ -201,14 +202,10 @@
 
  input_file = fopen(infilename,"r");
  if (input_file == NULL) {
-  perror("Can't open input file");
-  return (1);
+	perror("Can't open input file");
+	exit(2);
  }	
 
- if ((f = open(seekfilename,O_WRONLY|O_TRUNC|O_CREAT, 0644)) < 0) {
-  perror("Can't open seek file");
-  return (1);
- }
 
  srcinfo.err = jpeg_std_error(&jsrcerr);
  jpeg_create_decompress(&srcinfo);
@@ -232,14 +229,9 @@
   iv[i] = (((short**)(((void**)(((void**)coef_arrays)[0]))[0]))[0])[i]; 
  }
 
- pass = getpass("Passphrase: ");
- if (strlen(pass) == 0) {
-  fprintf(stderr,"Nothing done\n");
-  exit(0);
- }  
  if (strlen(pass) > 120) {
   fprintf(stderr,"Truncated to 120 characters\n");
-  pass[120] = 0;
+	exit(3);
  }
 
  Blowfish_ExpandUserKey(pass,strlen(pass),bkey);
@@ -266,8 +258,8 @@
   v = 0;
   for(j=0;j<8;j++) {
    if ((b = get_bit()) < 0) {
-    fprintf(stderr,"File not completely recovered\n");
-    exit(1);
+	result = 1;
+	goto end;
    }
    b = b << j;
    v |= b;
@@ -291,25 +283,27 @@
  while(count < length) {
   for(j=0;j<8;j++) {
    if ((b = get_bit()) < 0) {
-    fprintf(stderr,"File not completely recovered\n");
-    exit(1);
+	result = 1;
+	goto end;
    }
    b ^= get_code_bit(1);
    v = v << 1;
    v |= b;
    tail--;
   }
-  write(f,&v,1);
+  //write(f,&v,1);
   count++;
  }
 
+end:
+
  jpeg_finish_decompress(&srcinfo);
  jpeg_destroy_decompress(&srcinfo);
 
- close(f);
+ //close(f);
  fclose(input_file);
 
- return(0);
+ return result;
 
 }
 
@@ -332,14 +326,40 @@
 static void usage()
 {
  fprintf(stderr,"Usage:\n\n\
-jpseek input-file seek-file\n\n");
+jpseek input-file passwords-list-file\n\n");
  exit(1);
 }
 
 int main(int argc, char **argv)
 {
- intro();
- if (argc != 3) usage();
- return (jpseek(argv[1],argv[2]));
+ if (argc != 3) { 
+   usage(); exit(10);
+}
+
+ char buff[128] = {0};
+
+ FILE* pass_file = fopen(argv[2],"r");
+ if (pass_file == NULL) {
+	perror("Can't open passwords file");
+	exit(20);
+ }	
+
+ long counter = 0;
+ while (fgets(buff, 120, pass_file) != NULL) {
+	// get rid of \n
+	buff[strcspn(buff, "\n")] = 0;
+	counter++;
+
+	printf("[trying=%s]\n", buff);
+	if (jpseek(argv[1], buff) == 0) {
+		puts(buff);
+	}
+
+	if ((counter % 100000) == 0) {
+		fprintf(stderr, "(stderr) currently at %s\n", buff);
+	}
+ }
+
+ return 0;
 }
