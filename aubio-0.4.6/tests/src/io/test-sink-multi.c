#include <aubio.h>
#include "utils_tests.h"

// same as test-sink.c, but uses aubio_source_do_multi to read multiple
// channels

int main (int argc, char **argv)
{
  sint_t err = 0;

  if (argc < 3) {
    err = 2;
    PRINT_ERR("not enough arguments\n");
    PRINT_MSG("usage: %s <input_path> <output_path> [samplerate] [channels] [hop_size]\n", argv[0]);
    return err;
  }

  uint_t samplerate = 0;
  uint_t channels = 0;
  uint_t hop_size = 512;
  uint_t n_frames = 0, read = 0;

  char_t *source_path = argv[1];
  char_t *sink_path = argv[2];

  if ( argc >= 4 ) samplerate = atoi(argv[3]);
  if ( argc >= 5 ) channels = atoi(argv[4]);
  if ( argc >= 6 ) hop_size = atoi(argv[5]);
  if ( argc >= 7 ) {
    err = 2;
    PRINT_ERR("too many arguments\n");
    return err;
  }

  aubio_source_t *i = new_aubio_source(source_path, samplerate, hop_size);
  if (!i) { err = 1; goto beach_source; }

  if (samplerate == 0 ) samplerate = aubio_source_get_samplerate(i);
  if (channels == 0 ) channels = aubio_source_get_channels(i);

  fmat_t *mat = new_fmat(channels, hop_size);
  if (!mat) { err = 1; goto beach_fmat; }

  aubio_sink_t *o = new_aubio_sink(sink_path, 0);
  if (!o) { err = 1; goto beach_sink; }
  err = aubio_sink_preset_samplerate(o, samplerate);
  if (err) { goto beach; }
  err = aubio_sink_preset_channels(o, channels);
  if (err) { goto beach; }

  do {
    aubio_source_do_multi(i, mat, &read);
    aubio_sink_do_multi(o, mat, read);
    n_frames += read;
  } while ( read == hop_size );

  PRINT_MSG("read %d frames at %dHz in %d channels (%d blocks) from %s written to %s\n",
      n_frames, samplerate, channels, n_frames / hop_size,
      source_path, sink_path);
  PRINT_MSG("wrote %s with %dHz in %d channels\n", sink_path,
      aubio_sink_get_samplerate(o),
      aubio_sink_get_channels(o) );

beach:
  del_aubio_sink(o);
beach_sink:
  del_fmat(mat);
beach_fmat:
  del_aubio_source(i);
beach_source:
  return err;
}
