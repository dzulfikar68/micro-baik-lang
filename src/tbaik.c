// ---------------------------------------------------------------------
//
// BAIK (Bahasa Anak Indonesia untuk Komputer) versi 8.15
// Object Oriented Programming Language
//
// BAIK versi 8.15 Ready for Commercial class
// Professional Web, Desktop and Cloud Computing
// Haris Script Language : Multi Platform Version

// Copyright Haris Hasanudin (2014)

// Last : 2014-5-15 Fix Sub Lyr Param

// ---------------------------------------------------------------------

// Kupersembahkan untuk istriku tercinta Masako, anakku tersayang Takumi
// dan Tomoki serta seluruh putra putri Indonesia

// ---------------------------------------------------------------------

// ======================================================================================
// Lisensi untuk BAIK versi 8.15 adalah sebagai berikut :
// ======================================================================================
// 0. Desain Program dan Kode Sumber BAIK versi 8 memiliki hak cipta dan bersifat rahasia.
//    - BAIK Software Design is confidential and copyrighted.
// 1. Program dan Kode Sumber BAIK versi 8 disediakan seadanya dan tanpa kewenangan
//    atas segala kesalahan/kerugian yang ditimbulkan akibat penggunaannya.
//    - Software is provided "AS IS" and "NO WARRANTY".
// 2. Anda dibenarkan untuk mengkopi/mendistribusikan kembali Program dan atau Kode Sumber
//    BAIK versi 6 jika tanpa perubahan apapun.
//    - You can redistribute/copy software without modification.
// 3. Anda dibenarkan memodifikasi Program dan atau Kode Sumber BAIK versi 8 jika ada
//    ijin tertulis dari pencipta BAIK.
//    - You can modify or add feature(s) to this software with agreement from Author.
// 4. Segala persengketaan yang berkenaan dengan persetujuan lisensi ini akan diadili
//    dengan hukum yang ada di Jepang atau Indonesia menurut kehendak pencipta BAIK.
//    - Any action related to this Agreement will be governed by Indonesia/Japan laws.
// 5. Semua software BAIK mengikuti aturan hukum ekspor di Jepang atau Indonesia
//    - All Software are subject to Indonesia/Japan export control laws.
// 6. Anda tidak dibenarkan untuk menjual atau menjadi broker penjualan software BAIK.
//    - You are not allowed to sell or resell this software.
// 7. Anda dibenarkan untuk menggunakan software BAIK untuk kepentingan pribadi, pendidikan
//    maupun komersial.
//    - You may use this software for personal, educational or commercial use.
// 8. Anda dibenarkan untuk menggunakan kode-kode dalam bahasa BAIK yang anda ciptakan
//    untuk kepentingan pribadi, pendidikan maupun komersial.
//    - You may use all codes that you created by BAIK language for personal, educational
//    or commercial use.
// 9. Anda tidak dibenarkan untuk mengaku sebagai pemilik hak cipta software BAIK
//    - You are not allowed to claim the copyright of this software.
// 10. Persetujuan lisensi ini mungkin akan diubah pada masa yang akan datang dengan
//    pemberitahuan oleh pencipta BAIK.
//    -This Agreement may be changed in the future by Author announcement.
// ======================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/stat.h>

#include <time.h>
#include <math.h>
#include <limits.h>
#include <errno.h>

#include "baik_header.h"
// ------------------------------------------------------------------

extern void BaikGarbageCollection(void);
extern VAL_LABEL returnVal;

#ifdef WIN_NONCONSOLE
int main(int argc, char *argv[]);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int showCmd)
{
  int ac = 0;
  char **av = NULL;

  char cmd[] = "baik.exe";
  int r = 0;
  int len = 0;
  int pos = 0;

  len = lstrlen(lpszCmdLine);

  pos = 0;
  for (ac = 1; pos < len; ac++)
  {
    while (lpszCmdLine[pos] == ' ')
      pos++;

    if (pos < len)
      ;
    else
      break;

    if (lpszCmdLine[pos] == '\"')
    {
      pos++;
      while (pos < len && lpszCmdLine[pos] != '\"')
        pos++;
    }
    else
    {
      while (pos < len && lpszCmdLine[pos] != ' ')
        pos++;
    }
    pos++;
  }

  av = (char **)malloc(sizeof(char *) * ac);
  av[0] = cmd;
  pos = 0;
  for (ac = 1; pos < len; ac++)
  {
    while (lpszCmdLine[pos] == ' ')
      pos++;

    if (pos < len)
      ;
    else
      break;

    if (lpszCmdLine[pos] == '\"')
    {
      pos++;
      av[ac] = lpszCmdLine + pos;
      while (pos < len && lpszCmdLine[pos] != '\"')
        pos++;
    }
    else
    {
      av[ac] = lpszCmdLine + pos;
      while (pos < len && lpszCmdLine[pos] != ' ')
        pos++;
    }
    lpszCmdLine[pos] = '\0';
    pos++;
  }

  r = main(ac, av);

  free(av);

  return r;
}

#endif

int main(int argc, char *argv[])
{
  FILE *fp = NULL;
  struct stat st;
  char **get = NULL;
  int useDefault = 0;
  int i = 0;
  char cfilename[MAX_STRING_LEN];
  char command[MAX_STRING_LEN];

#ifdef USE_GTK2
  gchar *mainFile = "./utama.ina";
#endif

  /* init random            */
  init_rnd((unsigned long)time(NULL));

  if (argc < 2)
  {
#ifdef USE_GTK2
    if (stat(mainFile, &st) != 0)
    {
      showBaikVersion();
      exit(0);
    }
    // printf("stat default OK\n");
    if ((fp = fopen(mainFile, "r")) == NULL)
    {
      showBaikVersion();
      exit(0);
    }
    useDefault = 1;
    // printf("open default OK\n");
#else
    char repl_type[256];
    while (1)
    {
      printf("\nbaik >> ");
      scanf("%[^\n]%*c", repl_type);
      repl(repl_type);
    }
    exit(0);
#endif
  }

  if (argc > 1 && strncmp(argv[1], "-v", 2) == 0)
  {
    showBaikVersion();
  }

  if (argc > 1 && strncmp(argv[1], "-h", 2) == 0)
  {
    showBaikVersion();
  }

  if (argc > 1 && strncmp(argv[1], "-w", 2) != 0)
  {
    if (!useDefault)
    {
      if (stat(argv[1], &st) != 0)
      {
        fprintf(stderr, "File Source tidak ditemukan: %s\n", argv[1]);
        exit(0);
      }

      if ((fp = fopen(argv[1], "r")) == NULL)
      {
        fprintf(stderr, "tidak bisa buka File Source.\n");
        exit(0);
      }
    }
  }
  else if (argc > 1 && strncmp(argv[1], "-w", 2) == 0)
  {
    if (stat(argv[2], &st) != 0)
    {
      fprintf(stderr, "File Source tidak ditemukan: %s\n", argv[2]);
      exit(0);
    }
    if ((fp = fopen(argv[2], "r")) == NULL)
    {
      fprintf(stderr, "tidak bisa buka File Source.\n");
      exit(0);
    }
  }

  pg.source = (char *)calloc(st.st_size + 1, sizeof(char));
  if (pg.source == NULL)
  {
    fprintf(stderr, "kesalahan internal: calloc error\n");
    exit(0);
  }

  pg.pt = 0;
  pg.back_pt = 0;

  fread(pg.source, 1, st.st_size, fp);

  // ------------------------------------------------------------
  tmp_pg.source = (char *)calloc(st.st_size + 1, sizeof(char));
  if (tmp_pg.source == NULL)
  {
    fprintf(stderr, "kesalahan internal: calloc error\n");
    exit(-1);
  }

  fread(tmp_pg.source, 1, st.st_size, fp);

// ------------------------------------------------------------
#ifdef USE_GTK2
  tmp_source_lyr = (char *)calloc(st.st_size + 1, sizeof(char));
  if (tmp_source_lyr == NULL)
  {
    fprintf(stderr, "kesalahan internal: calloc error\n");
    exit(-1);
  }

  fread(tmp_source_lyr, 1, st.st_size, fp);
#endif
  // ------------------------------------------------------------

  fclose(fp);

  // if(strncmp(argv[1], "-c", 2) == 0) {
  //   if(strlen(argv[2]) > 0) {
  //     fprintf( stderr, "buat c source file : %s ...\n", argv[2]);

  //   get = split(argv[2], ".");
  //     if(get != NULL) {
  //       i=0;
  //       while(get[i] != NULL && strlen(get[i]) > 0) {
  //         i++;
  //       }
  //     }
  //   memset(&cfilename, '\0', sizeof(cfilename));
  //   //printf("i=%d\n", i);
  //   if(i == 2) {
  //       sprintf(cfilename,"%s.c", get[0]);
  //   } else if(i == 3) {
  //       sprintf(cfilename,"%s.%s.c", get[0],get[1]);
  //   } else if(i == 4) {
  //       sprintf(cfilename,"%s.%s.%s.c", get[0],get[1],get[2]);
  //   }

  //   split_free(get);

  //     createCsource(cfilename);
  //   }
  //   exit( 0 );
  // }

  if (strncmp(argv[1], "-w", 2) == 0)
  {
    if (strlen(argv[2]) > 0)
    {
      fprintf(stderr, "buat webassembly file : %s ...\n", argv[2]);

      get = split(argv[2], ".");
      if (get != NULL)
      {
        i = 0;
        while (get[i] != NULL && strlen(get[i]) > 0)
        {
          i++;
        }
      }
      memset(&cfilename, '\0', sizeof(cfilename));
      // memset(&command, '\0', sizeof(command));
      //printf("i=%d\n", i);
      if (i == 2)
      {
        sprintf(cfilename, "%s.c", get[0]);
      }
      else if (i == 3)
      {
        sprintf(cfilename, "%s.%s.c", get[0], get[1]);
      }
      else if (i == 4)
      {
        sprintf(cfilename, "%s.%s.%s.c", get[0], get[1], get[2]);
      }

      createCsource(cfilename);
      strcpy(command, "emcc -w -o ");
      strcat(command, get[0]);
      strcat(command, ".js ");
      strcat(command, "-DIOS -I /usr/src/baik ");
      strcat(command, cfilename);
      strcat(command, " /usr/src/baik/baik_ident.c /usr/src/baik/baik_stack.c /usr/src/baik/baik_expression.c /usr/src/baik/baik_compare.c /usr/src/baik/baik_factor.c /usr/src/baik/interpreter.c /usr/src/baik/interpreterSub.c /usr/src/baik/interpreterClass.c -lpthread -lm -s WASM=1");
      // printf(command);
      split_free(get);
      system(command);
    }
    exit(0);
  }

  BaikInit();

  // Read Include File and add into Main Prog
  IncludeCodeReader();

  pg.pt = 0;
  pg.back_pt = 0;
  memset(&lex, 0, sizeof(BAIK_LEX));

  ReadSource();

  pg.pt = 0;
  pg.back_pt = 0;
  memset(&lex, 0, sizeof(BAIK_LEX));

  memset(&returnVal, '\0', sizeof(returnVal));
  do
  {
    Interpreter();
  } while (lex.type != _EOF);

  BaikGarbageCollection();

  return 0;
}

// ------------------------------------------------------------------
// Software BAIK ini dilindungi Hak Cipta dan Undang-Undang ////////
// ------------------------------------------------------------------
