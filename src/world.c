#include "config.h"
#include "noise.h"
#include "world.h"

void create_world1(int p, int q, world_func func, void *arg)
{
   int dx;
   int pad = 1;
   for (dx = -pad; dx < CHUNK_SIZE + pad; dx++) {
      int dz;
      for (dz = -pad; dz < CHUNK_SIZE + pad; dz++) {
         int flag = 1;
         if (dx < 0 || dz < 0 || dx >= CHUNK_SIZE || dz >= CHUNK_SIZE) {
            flag = -1;
         }

         {
            int y;
            int x = p * CHUNK_SIZE + dx;
            int z = q * CHUNK_SIZE + dz;
            float f = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2);
            float g = simplex2(-x * 0.01, -z * 0.01, 2, 0.9, 2);
            int mh = g * 32 + 16;
            int h = f * mh;
            int w = 1;
            int t = 12;
            if (h <= t) {
               h = t;
               w = 2;
            }
            // sand and grass terrain
            for (y = 0; y < h; y++) {
               func(x, y, z, w * flag, arg);
            }
            if (w == 1) {
               int ok = SHOW_TREES;
               if (SHOW_PLANTS) {
                  // grass
                  if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                     func(x, h, z, 17 * flag, arg);
                  }
                  // flowers
                  if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                     int w = 18 + simplex2(x * 0.1, z * 0.1, 4, 0.8, 2) * 7;
                     func(x, h, z, w * flag, arg);
                  }
               }
               // trees
               if (dx - 4 < 0 || dz - 4 < 0 ||
                     dx + 4 >= CHUNK_SIZE || dz + 4 >= CHUNK_SIZE)
               {
                  ok = 0;
               }
               if (ok && simplex2(x, z, 6, 0.5, 2) > 0.84) {
                  int y;
                  for (y = h + 3; y < h + 8; y++) {
                     int ox;
                     for (ox = -3; ox <= 3; ox++) {
                        int oz;
                        for (oz = -3; oz <= 3; oz++) {
                           int d = (ox * ox) + (oz * oz) +
                              (y - (h + 4)) * (y - (h + 4));
                           if (d < 11) {
                              func(x + ox, y, z + oz, 15, arg);
                           }
                        }
                     }
                  }
                  for (y = h; y < h + 7; y++) {
                     func(x, y, z, 5, arg);
                  }
               }
            }
            // clouds
            if (SHOW_CLOUDS) {
               int y;
               for (y = 64; y < 72; y++) {
                  if (simplex3(
                           x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
                  {
                     func(x, y, z, 16 * flag, arg);
                  }
               }
            }
         }
      }
   }
}

void biome0(int x, int z, int flag, world_func func, void *arg)
{
   int y;
   float f = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2);
   float g = simplex2(-x * 0.01, -z * 0.01, 2, 0.9, 2);
   int mh = g * 32 + 16;
   int h = f * mh;
   int w = 1;
   int t = 12;
   if (h <= t) {
      h = t;
      w = 2;
   }
   // sand and grass terrain
   for (y = 0; y < h; y++) {
      func(x, y, z, w * flag, arg);
   }
   if (w == 1) {
      int ok = 0;//SHOW_TREES;
      if (SHOW_PLANTS) {
         // grass
         if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
            func(x, h, z, 17 * flag, arg);
         }
         // flowers
         if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
            int w = 18 + simplex2(x * 0.1, z * 0.1, 4, 0.8, 2) * 7;
            func(x, h, z, w * flag, arg);
         }
      }
      // trees
      // if (dx - 4 < 0 || dz - 4 < 0 ||
      //     dx + 4 >= CHUNK_SIZE || dz + 4 >= CHUNK_SIZE)
      // {
      //     ok = 0;
      // }
      if (ok && simplex2(x, z, 6, 0.5, 2) > 0.84) {
         int y;
         for (y = h + 3; y < h + 8; y++) {
            int ox;
            for (ox = -3; ox <= 3; ox++) {
               int oz;
               for (oz = -3; oz <= 3; oz++) {
                  int d = (ox * ox) + (oz * oz) +
                     (y - (h + 4)) * (y - (h + 4));
                  if (d < 11) {
                     func(x + ox, y, z + oz, 15, arg);
                  }
               }
            }
         }

         for (y = h; y < h + 7; y++)
            func(x, y, z, 5, arg);
      }
   }
   // clouds
   if (SHOW_CLOUDS)
   {
      int y;
      for (y = 64; y < 72; y++)
      {
         if (simplex3(
                  x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
            func(x, y, z, 16 * flag, arg);
      }
   }
}

void biome1(int x, int z, int flag, world_func func, void *arg)
{
   int y;
   int lo = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2) * 8 + 8;
   int hi = simplex2(-x * 0.01, -z * 0.01, 4, 0.5, 2) * 32 + 32;
   int lookup[] = {3, 6, 11, 12, 13};
   for (y = 0; y < lo; y++)
      func(x, y, z, 6 * flag, arg);

   for (y = lo; y < hi; y++)
   {
      int i = simplex3(-x * 0.01, -y * 0.01, -z * 0.01, 4, 0.5, 2) * 10;
      int w = lookup[i % 5];
      if (simplex3(x * 0.01, y * 0.01, z * 0.01, 4, 0.5, 2) > 0.5)
         func(x, y, z, w * flag, arg);
   }

   if (SHOW_CLOUDS)
   {
      for (y = 64; y < 72; y++)
      {
         if (simplex3(
                  x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
            func(x, y, z, 16 * flag, arg);
      }
   }
}

void create_world2(int p, int q, world_func func, void *arg) {
   int dx;
    int pad = 1;
    for (dx = -pad; dx < CHUNK_SIZE + pad; dx++) {
       int dz;
        for (dz = -pad; dz < CHUNK_SIZE + pad; dz++) {
           int x, z, i;
            int flag = 1;
            if (dx < 0 || dz < 0 || dx >= CHUNK_SIZE || dz >= CHUNK_SIZE) {
                flag = -1;
            }
            x = p * CHUNK_SIZE + dx;
            z = q * CHUNK_SIZE + dz;
            i = simplex2(-x * 0.001, -z * 0.001, 8, 0.5, 2) * 2;
            if (i == 0) biome0(x, z, flag, func, arg);
            else biome1(x, z, flag, func, arg);
        }
    }
}

void create_world(int p, int q, world_func func, void *arg) {
    create_world2(p, q, func, arg);
}
