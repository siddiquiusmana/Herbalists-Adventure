#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "leak_detector_c.c"
#include <math.h>

typedef struct plant {
  char *name;
  char *type;
  int abundance;
} plant;

typedef struct region {
  char *name;
  int nplants;
  int total_abundance;
  plant **plants;
} region;

typedef struct wishList {
  int nregions;
  region **regions;
  int picks;
} wishList;

typedef struct herbalist {
  char *name;
  wishList *visits;
} herbalist;

plant* createplant(char *name, char *type, int abundance);

plant** readplants(FILE* infile, int *plantCount);

region** readRegions(FILE* infile, int    *countRegions, plant** plantList, int plantCount);

herbalist* readherbalists(FILE* infile, int *herbalistCount, region** regionList, int countRegions);

void process_inputs(plant** plantList, int plantCount, region** regionList, int regionCount, herbalist* herbalistList, int herbalistCount );

void release_memory(plant** plantList, int plantCount, region** regionList, int    regionCount, herbalist* herbalistList, int herbalistCount);

int main()
{
  atexit(report_mem_leak);

  FILE *ifp = fopen("in.txt", "r");

  //plant reading
  int *plantCount = (int*) malloc(sizeof(int));
  char tempc[50];
  fscanf(ifp, "%d %s", plantCount, tempc);
  plant** plants = readplants(ifp, plantCount);

  //region reading
  int *regionCount = (int*) malloc(sizeof(int));
  fscanf(ifp, "%d %s", regionCount, tempc);
  region** regions = readRegions(ifp, regionCount, plants, *plantCount);

  //herbalist reading
  int *herbalistCount = (int*) malloc(sizeof(int));
  fscanf(ifp, "%d %s", herbalistCount, tempc);
  herbalist* herbalists = readherbalists(ifp, herbalistCount, regions, *regionCount);

  fclose(ifp);

  //Proper Storage Tester Code
  /*
  printf("%d herbalists\n\n", *herbalistCount);
  for(int i=0;i<*herbalistCount;i++)
  {
    printf("Name: %s\n", herbalists[i].name);
    printf("%d Regions:\n\n", herbalists[i].visits->nregions);
    for(int j=0;j<herbalists[i].visits->nregions;j++)
    {
      region* a = herbalists[i].visits->regions[j];
      printf("-------\nRegion #%d\n-------\n\tName: %s\n",j+1, a->name);
      printf("\tCount of plants: %d\n\t----------\n", a->nplants);
      for(int k=0;k<a->nplants;k++)
      {
        printf("\tplant#%d: Name: %s\ttype: %s\t\tabundance: %d\n", k+1, a->plants[k]->name,a->plants[k]->type,a->plants[k]->abundance);
      }
      printf("\n\tTotal abundance of Region: %d\n\n--------\n", a->total_abundance);
    }
    printf("\n");
  }
  */

  process_inputs(plants, *plantCount, regions, *regionCount, herbalists, *herbalistCount);
  release_memory(plants, *plantCount, regions, *regionCount, herbalists, *herbalistCount);

  free(plantCount);
  free(regionCount);
  free(herbalistCount);
}

plant* createplant(char *name, char *type, int abundance)
{
  plant *temp = (plant*) malloc(sizeof(plant));
  temp->type = (char*)malloc(50*sizeof(char));
  temp->name = (char*)malloc(50*sizeof(char));

  strcpy(temp->name, name);
  strcpy(temp->type, type);
  temp->abundance = abundance;

  return temp;
}

plant** readplants(FILE* infile, int *plantCount)
{
  plant** plants = (plant**)malloc((*plantCount)*sizeof(plant*));

  char name[50], type[50];
  int abundance;

  for(int i=0; i<*plantCount; i++)
  {
    fscanf(infile, "%s %s %d", name, type, &abundance);
    plants[i] = createplant(name, type, abundance);
  }
  return plants;
}

region** readRegions(FILE* infile, int    *countRegions, plant** plantList, int plantCount)
{
  region** regions = (region**)malloc(*countRegions*sizeof(region*));

  for(int i=0; i<*countRegions; i++)
  {
    regions[i] = (region*) malloc(sizeof(region));
    regions[i]->name = (char*) malloc(50*sizeof(char));
  }
  char tempc[50];
  for(int i=0;i<*countRegions;i++)
  {
    fscanf(infile, "%s", regions[i]->name);
    fscanf(infile, "%d %s", &regions[i]->nplants, tempc);

    plant** regionplants = (plant**) malloc((regions[i]->nplants)*sizeof(plant*));

    for(int j=0; j<regions[i]->nplants; j++)
    {
      char mtemp[50];
      fscanf(infile, "%s", mtemp);
      for(int k=0; k<plantCount; k++)
      {
        if(strcmp(plantList[k]->name, mtemp)==0)
        {
          regions[i]->total_abundance += plantList[k]->abundance;
          regionplants[j] = plantList[k];
        }
      }
    }
    regions[i]->plants = regionplants;
  }
  return regions;
}

herbalist* readherbalists(FILE* infile, int *herbalistCount, region** regionList, int countRegions)
{
  herbalist* herbalists = (herbalist*)malloc((*herbalistCount)*sizeof(herbalist));
  for(int i=0; i<*herbalistCount; i++)
  {
    char tempc[51];
    herbalists[i].name = (char*) malloc(51*sizeof(char));
    fscanf(infile, "%s", herbalists[i].name);
    herbalists[i].visits = (wishList*) malloc(sizeof(wishList));
    fscanf(infile, "%d %s", &herbalists[i].visits->picks, tempc);
    fscanf(infile, "%d %s", &herbalists[i].visits->nregions, tempc);
    herbalists[i].visits->regions = (region**) malloc((herbalists[i].visits->nregions)*sizeof(region*));
    for(int j=0; j<herbalists[i].visits->nregions; j++)
    {
      char rtemp[51];
      fscanf(infile, "%s", rtemp);
      for(int k=0; k<countRegions; k++)
      {
        if(strcmp(regionList[k]->name, rtemp)==0)
        {
          herbalists[i].visits->regions[j] = regionList[k];
        }
      }
    }
  }
  return herbalists;
}

void process_inputs(plant** plantList, int plantCount, region** regionList, int regionCount, herbalist* herbalistList, int herbalistCount)
{
  FILE *ofp = fopen("out.txt", "w");
  for(int i=0; i<herbalistCount; i++)
  {
    fprintf(ofp, "%s\n", herbalistList[i].name);
    printf("%s\n", herbalistList[i].name);
    wishList* a = herbalistList[i].visits;
    for(int j=0; j<a->nregions;j++)
    {
      fprintf(ofp, "%s\n", a->regions[j]->name);
      printf("%s\n", a->regions[j]->name);
      region* b = a->regions[j];
      for(int k=0;k<b->nplants;k++)
      {
        float ratio = (b->plants[k]->abundance*1.0) / b->total_abundance;
        int total_picks = (round(a->picks * ratio));
        if(total_picks)
        {
          fprintf(ofp, "%d %s\n", total_picks, b->plants[k]->name);
          printf("%d %s\n", total_picks, b->plants[k]->name);
        }
      }
    }
    if(i!=herbalistCount -1)
    {
      fprintf(ofp, "\n");
      printf("\n");
    }
  }
  fclose(ofp);
}

void release_memory(plant** plantList, int plantCount, region** regionList, int    regionCount, herbalist* herbalistList, int herbalistCount)
{
  //plant memory release
  for(int i=0; i<plantCount;i++)
  {
    free(plantList[i]->name);
    free(plantList[i]->type);
    free(plantList[i]);
  }
  free(plantList);

  //regions memory release
  for(int i=0;i<regionCount;i++)
  {
    free(regionList[i]->name);
    free(regionList[i]->plants);
    free(regionList[i]);
  }
  free(regionList);

  //herbalist memory release
  for(int i=0; i<herbalistCount; i++)
  {
    free(herbalistList[i].name);
    free(herbalistList[i].visits->regions);
    free(herbalistList[i].visits);
  }
  free(herbalistList);
}
