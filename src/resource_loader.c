#include "global.h"
#include "palette.h"
#include "resource_loader.h"
#include "util.h"
#include "gbagfx/gbagfx.h"

#define LOADER_DEBUG

static struct DataSource **loadedDataSources = NULL;
static size_t numLoadedDataSources = 0;

static struct GraphicsSource **loadedGfxSources = NULL;
static size_t numLoadedGfxSources = 0;

static void AddDataSource(struct DataSource *src)
{
    numLoadedDataSources++;
    loadedDataSources = realloc(loadedDataSources, numLoadedDataSources * sizeof(*loadedDataSources));
    if (!loadedDataSources)
        abort();
    loadedDataSources[numLoadedDataSources - 1] = src;
    src->loaded = TRUE;
}

static void AddGfxSource(struct GraphicsSource *gfx)
{
    numLoadedGfxSources++;
    loadedGfxSources = realloc(loadedGfxSources, numLoadedGfxSources * sizeof(*loadedGfxSources));
    if (!loadedGfxSources)
        abort();
    loadedGfxSources[numLoadedGfxSources - 1] = gfx;
    gfx->loaded = TRUE;
}

bool8 LoadDataFromFile(struct DataSource *src, void *dest)
{
    const char *path = src->path;
    if (!path)
        return FALSE;

    if (!src->data)
    {
        int size;

        src->data = ReadWholeFile(path, &size);

        if (src->data)
        {
            src->size = (size_t)size;

#ifdef LOADER_DEBUG
            printf("LoadDataFromFile: Loaded %s (size: %d)\n", path, src->size);
#endif

            AddDataSource(src);
        }
        else
        {
            fprintf(stderr, "LoadDataFromFile: Could not load %s\n", path);

            src->loaded = FALSE;
        }
    }

    if (!src->loaded)
        return FALSE;

    memcpy(dest, src->data, src->size);

    return TRUE;
}

static void *LoadImageData(const char *path, size_t *outSize, void **outPalette, size_t *outPaletteSize)
{
    struct PngToGbaOptions options = {};

    options.bitDepth = 4;

#ifdef LOADER_DEBUG
    printf("LoadImageData: %s\n", path);
#endif

    return HandlePngToGbaCommand(path, &options, outSize, outPalette, outPaletteSize);
}

static void *LoadImageData8bpp(const char *path, size_t *outSize, void **outPalette, size_t *outPaletteSize)
{
    struct PngToGbaOptions options = {};

    options.bitDepth = 8;

#ifdef LOADER_DEBUG
    printf("LoadImageData8bpp: %s\n", path);
#endif

    return HandlePngToGbaCommand(path, &options, outSize, outPalette, outPaletteSize);
}

bool8 LoadImage(struct GraphicsSource *gfx, void *dest)
{
    const char *path = gfx->path;
    if (!path)
        return FALSE;

    void *image = gfx->image;
    if (!image)
    {
        gfx->image = LoadImageData(path, &gfx->size, NULL, NULL);

        if (gfx->image)
        {
#ifdef LOADER_DEBUG
            printf("LoadImage: Loaded %s (size %u)\n", path, gfx->size);
#endif

            AddGfxSource(gfx);
        }
        else
        {
            fprintf(stderr, "LoadImage: Could not load %s\n", path);

            gfx->loaded = FALSE;
        }
    }

    if (!gfx->loaded)
        return FALSE;

    memcpy(dest, gfx->image, gfx->size);

    return TRUE;
}

bool8 LoadImage8bpp(struct GraphicsSource *gfx, void *dest)
{
    const char *path = gfx->path;
    if (!path)
        return FALSE;

    void *image = gfx->image;
    if (!image)
    {
        gfx->image = LoadImageData8bpp(path, &gfx->size, NULL, NULL);

        if (gfx->image)
        {
#ifdef LOADER_DEBUG
            printf("LoadImage8bpp: Loaded %s (size %u)\n", path, gfx->size);
#endif

            AddGfxSource(gfx);
        }
        else
        {
            fprintf(stderr, "LoadImage8bpp: Could not load %s\n", path);

            gfx->loaded = FALSE;
        }
    }

    if (!gfx->loaded)
        return FALSE;

    memcpy(dest, gfx->image, gfx->size);

    return TRUE;
}

bool8 LoadImageAndPaletteColors(struct GraphicsSource *gfx, void *dest, size_t palDest, unsigned maxColors)
{
    const char *path = gfx->path;
    if (!path)
        return FALSE;

    void *image = gfx->image;
    if (!image)
    {
        gfx->image = LoadImageData(path, &gfx->size, &gfx->palette, &gfx->paletteSize);

        if (gfx->image)
        {
#ifdef LOADER_DEBUG
            printf("LoadImageAndPalette: Loaded %s (size: %u, palette size: %u)\n", path, gfx->size, gfx->paletteSize);
#endif

            if (!gfx->palette)
                fprintf(stderr, "LoadImageAndPalette: Could not load palette from %s\n", path);

            AddGfxSource(gfx);
        }
        else {
            fprintf(stderr, "LoadImageAndPalette: Could not load %s\n", path);

            gfx->loaded = FALSE;
        }
    }

    if (!gfx->loaded)
        return FALSE;

    memcpy(dest, gfx->image, gfx->size);

    if (gfx->palette)
    {
        size_t loadSize = gfx->paletteSize;

        if (maxColors != 0)
            loadSize = min(maxColors * sizeof(u16), loadSize);

        LoadPalette(gfx->palette, palDest, loadSize);
    }

    return TRUE;
}

bool8 LoadImageAndPalette(struct GraphicsSource *gfx, void *dest, size_t palDest)
{
    return LoadImageAndPaletteColors(gfx, dest, palDest, 0);
}

bool8 LoadImageAndPaletteColors8bpp(struct GraphicsSource *gfx, void *dest, size_t palDest, unsigned maxColors)
{
    const char *path = gfx->path;
    if (!path)
        return FALSE;

    void *image = gfx->image;
    if (!image)
    {
        gfx->image = LoadImageData8bpp(path, &gfx->size, &gfx->palette, &gfx->paletteSize);

        if (gfx->image)
        {
#ifdef LOADER_DEBUG
            printf("LoadImageAndPalette8bpp: Loaded %s (size: %u, palette size: %u)\n", path, gfx->size, gfx->paletteSize);
#endif

            if (!gfx->palette)
                fprintf(stderr, "LoadImageAndPalette: Could not load palette from %s\n", path);

            AddGfxSource(gfx);
        }
        else
        {
            fprintf(stderr, "LoadImageAndPalette8bpp: Could not load %s\n", path);

            gfx->loaded = FALSE;
        }
    }

    if (!gfx->loaded)
        return FALSE;

    memcpy(dest, gfx->image, gfx->size);

    if (gfx->palette)
    {
        size_t loadSize = gfx->paletteSize;

        if (maxColors != 0)
            loadSize = min(maxColors * sizeof(u16), loadSize);

        LoadPalette(gfx->palette, palDest, loadSize);
    }

    return TRUE;
}

bool8 LoadImageAndPalette8bpp(struct GraphicsSource *gfx, void *dest, size_t palDest)
{
    return LoadImageAndPaletteColors8bpp(gfx, dest, palDest, 0);
}

bool8 LoadPaletteFromFile(struct DataSource *src, size_t dest, int numColors)
{
    const char *path = src->path;
    if (!path)
        return FALSE;

    if (!src->data)
    {
        // Load is done with size 0 so that the entire palette is loaded
        src->data = HandleJascToGbaPaletteCommand(path, 0, &src->size);

        if (src->data)
        {
#ifdef LOADER_DEBUG
            printf("LoadPaletteFromFile: Loaded %s (size: %d)\n", path, src->size);
#endif

            AddDataSource(src);
        }
        else
        {
            fprintf(stderr, "LoadPaletteFromFile: Could not load %s\n", path);

            src->loaded = FALSE;
        }
    }

    if (!src->loaded)
        return FALSE;

    size_t loadSize = src->size;

    if (numColors > 0)
        loadSize = min(numColors * sizeof(u16), loadSize);

    LoadPalette(src->data, dest, loadSize);

    return TRUE;
}

static void FlushLoadedDataSources(void)
{
    if (!numLoadedDataSources)
        return;

#ifdef LOADER_DEBUG
    printf("Deleting %u resource(s)...\n", numLoadedDataSources);
#endif

    for (size_t i = 0; i < numLoadedDataSources; i++)
    {
        struct DataSource *src = loadedDataSources[i];
        if (src->loaded)
        {
#ifdef LOADER_DEBUG
            printf("%s (size: %u)\n", src->path, src->size);
#endif
            free(src->data);
            src->data = NULL;
            src->size = 0;
            src->loaded = false;
        }
    }

#ifdef LOADER_DEBUG
    printf("Deleted %u resource(s)\n", numLoadedDataSources);
#endif

    free(loadedDataSources);

    loadedDataSources = NULL;

    numLoadedDataSources = 0;
}

static void FlushLoadedGfxSources(void)
{
    if (!numLoadedGfxSources)
        return;

#ifdef LOADER_DEBUG
    printf("Deleting %u graphic(s)...\n", numLoadedGfxSources);
#endif

    for (size_t i = 0; i < numLoadedGfxSources; i++)
    {
        struct GraphicsSource *gfx = loadedGfxSources[i];
        if (gfx->loaded)
        {
#ifdef LOADER_DEBUG
            printf("%s (size: %u, palette size: %u)\n", gfx->path, gfx->size, gfx->paletteSize);
#endif
            free(gfx->image);
            free(gfx->palette);
            gfx->image = NULL;
            gfx->size = 0;
            gfx->paletteSize = 0;
            gfx->loaded = false;
        }
    }

#ifdef LOADER_DEBUG
    printf("Deleted %u graphic(s)\n", numLoadedGfxSources);
#endif

    free(loadedGfxSources);

    loadedGfxSources = NULL;

    numLoadedGfxSources = 0;
}

void FreeLoadedAssets(void)
{
    FlushLoadedDataSources();
    FlushLoadedGfxSources();
}
