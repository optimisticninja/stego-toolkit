diff --git a/jpseek.c b/jpseek.c
index 3573426..6ad8aea 100644
--- a/jpseek.c
+++ b/jpseek.c
@@ -205,7 +205,7 @@ static int jpseek (const char* infilename,
   return (1);
  }	
 
- if ((f = open(seekfilename,O_WRONLY|O_TRUNC|O_CREAT)) < 0) {
+ if ((f = open(seekfilename,O_WRONLY|O_TRUNC|O_CREAT, 0644)) < 0) {
   perror("Can't open seek file");
   return (1);
  }
