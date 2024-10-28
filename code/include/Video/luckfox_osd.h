
#include "sample_comm.h"
#include "osd/osd.h"

#if __cplusplus
extern "C" {
#endif

int rkipc_osd_cover_create(int id, osd_data_s *osd_data);

int rkipc_osd_cover_destroy(int id);

int rkipc_osd_bmp_create(int id, osd_data_s *osd_data);

int rkipc_osd_bmp_destroy(int id);

int rkipc_osd_bmp_change(int id, osd_data_s *osd_data);

int rkipc_osd_init();

int rkipc_osd_deinit();

#if __cplusplus
}
#endif







