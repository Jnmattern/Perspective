#include <pebble.h>

#define M_PI		3.14159265358979323846

#define SIZE 40
#define OFFSET 5
#define ZPOS -20
#define EYEZ 200
#define SHIFT 8

#define TIMERDELAY 10

enum {
  CONFIG_KEY_NIGHTSTART = 3333,
  CONFIG_KEY_NIGHTSTOP = 3334
};

static const int32_t TRIG_MAX_ANGLE_BY_4 = TRIG_MAX_ANGLE / 4;

static Window *window;
static Layer *rootLayer, *layer;
static AppTimer *timer;
static int32_t a=0, b=0, c=0;
static int32_t cosa, sina, cosb, sinb, cosc, sinc;
static int hour;
static int d[6];
static int digitOffsetX[6], digitOffsetY[6];
static GRect fullScreenRect = { { 0, 0 }, { 144, 168 } };
static int nightStartHour = 19, nightStopHour = 7;

static const uint16_t __ACOS[1025] = {
  32768, 32115, 31845, 31638, 31463, 31309, 31169, 31041, 30921, 30809, 30703, 30602, 30505, 30412, 30323, 30237,
  30153, 30072, 29994, 29917, 29843, 29770, 29699, 29629, 29561, 29495, 29429, 29365, 29302, 29240, 29179, 29119,
  29060, 29002, 28945, 28889, 28833, 28778, 28724, 28670, 28617, 28565, 28513, 28462, 28412, 28362, 28312, 28263,
  28215, 28167, 28120, 28072, 28026, 27980, 27934, 27889, 27844, 27799, 27755, 27711, 27667, 27624, 27581, 27539,
  27496, 27454, 27413, 27372, 27330, 27290, 27249, 27209, 27169, 27129, 27090, 27051, 27012, 26973, 26934, 26896,
  26858, 26820, 26783, 26745, 26708, 26671, 26634, 26597, 26561, 26525, 26489, 26453, 26417, 26382, 26346, 26311,
  26276, 26241, 26206, 26172, 26137, 26103, 26069, 26035, 26001, 25968, 25934, 25901, 25868, 25835, 25802, 25769,
  25736, 25703, 25671, 25639, 25607, 25574, 25542, 25511, 25479, 25447, 25416, 25384, 25353, 25322, 25291, 25260,
  25229, 25198, 25168, 25137, 25107, 25076, 25046, 25016, 24986, 24956, 24926, 24896, 24867, 24837, 24807, 24778,
  24749, 24719, 24690, 24661, 24632, 24603, 24574, 24546, 24517, 24488, 24460, 24431, 24403, 24375, 24346, 24318,
  24290, 24262, 24234, 24206, 24179, 24151, 24123, 24095, 24068, 24040, 24013, 23986, 23958, 23931, 23904, 23877,
  23850, 23823, 23796, 23769, 23742, 23716, 23689, 23662, 23636, 23609, 23583, 23557, 23530, 23504, 23478, 23452,
  23425, 23399, 23373, 23347, 23321, 23296, 23270, 23244, 23218, 23193, 23167, 23141, 23116, 23090, 23065, 23040,
  23014, 22989, 22964, 22938, 22913, 22888, 22863, 22838, 22813, 22788, 22763, 22738, 22714, 22689, 22664, 22639,
  22615, 22590, 22565, 22541, 22516, 22492, 22468, 22443, 22419, 22394, 22370, 22346, 22322, 22298, 22273, 22249,
  22225, 22201, 22177, 22153, 22129, 22105, 22082, 22058, 22034, 22010, 21987, 21963, 21939, 21916, 21892, 21868,
  21845, 21821, 21798, 21774, 21751, 21728, 21704, 21681, 21658, 21634, 21611, 21588, 21565, 21542, 21518, 21495,
  21472, 21449, 21426, 21403, 21380, 21357, 21334, 21311, 21289, 21266, 21243, 21220, 21197, 21175, 21152, 21129,
  21107, 21084, 21061, 21039, 21016, 20994, 20971, 20949, 20926, 20904, 20881, 20859, 20836, 20814, 20792, 20769,
  20747, 20725, 20702, 20680, 20658, 20636, 20614, 20591, 20569, 20547, 20525, 20503, 20481, 20459, 20437, 20415,
  20393, 20371, 20349, 20327, 20305, 20283, 20261, 20240, 20218, 20196, 20174, 20152, 20131, 20109, 20087, 20065,
  20044, 20022, 20000, 19979, 19957, 19935, 19914, 19892, 19871, 19849, 19827, 19806, 19784, 19763, 19741, 19720,
  19699, 19677, 19656, 19634, 19613, 19591, 19570, 19549, 19527, 19506, 19485, 19463, 19442, 19421, 19400, 19378,
  19357, 19336, 19315, 19294, 19272, 19251, 19230, 19209, 19188, 19167, 19145, 19124, 19103, 19082, 19061, 19040,
  19019, 18998, 18977, 18956, 18935, 18914, 18893, 18872, 18851, 18830, 18809, 18788, 18767, 18746, 18726, 18705,
  18684, 18663, 18642, 18621, 18600, 18579, 18559, 18538, 18517, 18496, 18475, 18455, 18434, 18413, 18392, 18372,
  18351, 18330, 18309, 18289, 18268, 18247, 18227, 18206, 18185, 18164, 18144, 18123, 18103, 18082, 18061, 18041,
  18020, 17999, 17979, 17958, 17938, 17917, 17896, 17876, 17855, 17835, 17814, 17793, 17773, 17752, 17732, 17711,
  17691, 17670, 17650, 17629, 17609, 17588, 17568, 17547, 17527, 17506, 17486, 17465, 17445, 17424, 17404, 17383,
  17363, 17342, 17322, 17301, 17281, 17261, 17240, 17220, 17199, 17179, 17158, 17138, 17117, 17097, 17077, 17056,
  17036, 17015, 16995, 16975, 16954, 16934, 16913, 16893, 16873, 16852, 16832, 16811, 16791, 16771, 16750, 16730,
  16710, 16689, 16669, 16648, 16628, 16608, 16587, 16567, 16546, 16526, 16506, 16485, 16465, 16445, 16424, 16404,
  16384, 16363, 16343, 16322, 16302, 16282, 16261, 16241, 16221, 16200, 16180, 16159, 16139, 16119, 16098, 16078,
  16057, 16037, 16017, 15996, 15976, 15956, 15935, 15915, 15894, 15874, 15854, 15833, 15813, 15792, 15772, 15752,
  15731, 15711, 15690, 15670, 15650, 15629, 15609, 15588, 15568, 15547, 15527, 15506, 15486, 15466, 15445, 15425,
  15404, 15384, 15363, 15343, 15322, 15302, 15281, 15261, 15240, 15220, 15199, 15179, 15158, 15138, 15117, 15097,
  15076, 15056, 15035, 15015, 14994, 14974, 14953, 14932, 14912, 14891, 14871, 14850, 14829, 14809, 14788, 14768,
  14747, 14726, 14706, 14685, 14664, 14644, 14623, 14603, 14582, 14561, 14540, 14520, 14499, 14478, 14458, 14437,
  14416, 14395, 14375, 14354, 14333, 14312, 14292, 14271, 14250, 14229, 14208, 14188, 14167, 14146, 14125, 14104,
  14083, 14062, 14041, 14021, 14000, 13979, 13958, 13937, 13916, 13895, 13874, 13853, 13832, 13811, 13790, 13769,
  13748, 13727, 13706, 13685, 13664, 13643, 13622, 13600, 13579, 13558, 13537, 13516, 13495, 13473, 13452, 13431,
  13410, 13389, 13367, 13346, 13325, 13304, 13282, 13261, 13240, 13218, 13197, 13176, 13154, 13133, 13111, 13090,
  13068, 13047, 13026, 13004, 12983, 12961, 12940, 12918, 12896, 12875, 12853, 12832, 12810, 12788, 12767, 12745,
  12723, 12702, 12680, 12658, 12636, 12615, 12593, 12571, 12549, 12527, 12506, 12484, 12462, 12440, 12418, 12396,
  12374, 12352, 12330, 12308, 12286, 12264, 12242, 12220, 12198, 12176, 12153, 12131, 12109, 12087, 12065, 12042,
  12020, 11998, 11975, 11953, 11931, 11908, 11886, 11863, 11841, 11818, 11796, 11773, 11751, 11728, 11706, 11683,
  11660, 11638, 11615, 11592, 11570, 11547, 11524, 11501, 11478, 11456, 11433, 11410, 11387, 11364, 11341, 11318,
  11295, 11272, 11249, 11225, 11202, 11179, 11156, 11133, 11109, 11086, 11063, 11039, 11016, 10993, 10969, 10946,
  10922, 10899, 10875, 10851, 10828, 10804, 10780, 10757, 10733, 10709, 10685, 10662, 10638, 10614, 10590, 10566,
  10542, 10518, 10494, 10469, 10445, 10421, 10397, 10373, 10348, 10324, 10299, 10275, 10251, 10226, 10202, 10177,
  10152, 10128, 10103, 10078, 10053, 10029, 10004, 9979, 9954, 9929, 9904, 9879, 9854, 9829, 9803, 9778,
  9753, 9727, 9702, 9677, 9651, 9626, 9600, 9574, 9549, 9523, 9497, 9471, 9446, 9420, 9394, 9368,
  9342, 9315, 9289, 9263, 9237, 9210, 9184, 9158, 9131, 9105, 9078, 9051, 9025, 8998, 8971, 8944,
  8917, 8890, 8863, 8836, 8809, 8781, 8754, 8727, 8699, 8672, 8644, 8616, 8588, 8561, 8533, 8505,
  8477, 8449, 8421, 8392, 8364, 8336, 8307, 8279, 8250, 8221, 8193, 8164, 8135, 8106, 8077, 8048,
  8018, 7989, 7960, 7930, 7900, 7871, 7841, 7811, 7781, 7751, 7721, 7691, 7660, 7630, 7599, 7569,
  7538, 7507, 7476, 7445, 7414, 7383, 7351, 7320, 7288, 7256, 7225, 7193, 7160, 7128, 7096, 7064,
  7031, 6998, 6965, 6932, 6899, 6866, 6833, 6799, 6766, 6732, 6698, 6664, 6630, 6595, 6561, 6526,
  6491, 6456, 6421, 6385, 6350, 6314, 6278, 6242, 6206, 6170, 6133, 6096, 6059, 6022, 5984, 5947,
  5909, 5871, 5833, 5794, 5755, 5716, 5677, 5638, 5598, 5558, 5518, 5477, 5437, 5395, 5354, 5313,
  5271, 5228, 5186, 5143, 5100, 5056, 5012, 4968, 4923, 4878, 4833, 4787, 4741, 4695, 4647, 4600,
  4552, 4504, 4455, 4405, 4355, 4305, 4254, 4202, 4150, 4097, 4043, 3989, 3934, 3878, 3822, 3765,
  3707, 3648, 3588, 3527, 3465, 3402, 3338, 3272, 3206, 3138, 3068, 2997, 2924, 2850, 2773, 2695,
  2614, 2530, 2444, 2355, 2262, 2165, 2064, 1958, 1846, 1726, 1598, 1458, 1304, 1129, 922, 652, 0
};

typedef struct {
  int x, y, z;
} GPoint3;

char digit[810] = {	 // 10 x 9 x 9
                     // 0
  0,0,1,1,1,1,1,0,0,
  0,1,1,1,1,1,1,1,0,
  1,1,1,0,0,0,1,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,1,0,0,0,1,1,1,
  0,1,1,1,1,1,1,1,0,
  0,0,1,1,1,1,1,0,0,

  // 1
  0,0,0,1,1,1,0,0,0,
  0,0,1,1,1,1,0,0,0,
  0,1,1,0,1,1,0,0,0,
  0,0,0,0,1,1,0,0,0,
  0,0,0,0,1,1,0,0,0,
  0,0,0,0,1,1,0,0,0,
  0,0,0,0,1,1,0,0,0,
  0,1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,1,

  // 2
  0,0,1,1,1,1,1,0,0,
  0,1,1,1,1,1,1,1,0,
  1,1,1,0,0,0,1,1,1,
  1,1,0,0,0,0,1,1,1,
  0,0,0,0,0,1,1,1,0,
  0,0,0,1,1,1,0,0,0,
  0,1,1,1,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,

  // 3
  0,0,1,1,1,1,1,0,0,
  0,1,1,1,1,1,1,1,0,
  1,1,1,0,0,0,1,1,1,
  1,1,0,0,0,0,1,1,1,
  0,0,0,0,1,1,1,1,0,
  1,1,0,0,0,0,1,1,1,
  1,1,1,0,0,0,1,1,1,
  0,1,1,1,1,1,1,1,0,
  0,0,1,1,1,1,1,0,0,

  // 4
  0,0,0,0,1,1,1,0,0,
  0,0,0,1,1,1,1,0,0,
  0,0,1,1,1,1,1,0,0,
  0,1,1,1,0,1,1,0,0,
  1,1,1,0,0,1,1,0,0,
  1,1,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,0,
  0,0,0,0,0,1,1,0,0,
  0,0,0,0,0,1,1,0,0,

  // 5
  1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,
  1,1,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,1,
  0,0,0,0,0,0,0,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,0,

  // 6
  0,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,
  1,1,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,0,

  // 7
  1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,
  0,0,0,0,0,0,0,1,1,
  0,0,0,0,0,0,1,1,0,
  0,0,0,0,0,1,1,0,0,
  0,0,0,0,1,1,0,0,0,
  0,0,0,1,1,0,0,0,0,
  0,0,0,1,1,0,0,0,0,
  0,0,0,1,1,0,0,0,0,

  // 8
  0,1,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,0,
  1,1,0,0,0,0,0,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,0,

  // 9
  0,1,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,1,
  1,1,0,0,0,0,0,1,1,
  1,1,0,0,0,0,0,1,1,
  0,1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,1,
  0,0,0,0,0,0,0,1,1,
  1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,0
};

#define DIGIT(num, x, y) (digit[(81*(num))+((y)*9)+(x)])

GPoint3 eye = { 0, 0, EYEZ };

GPoint3 pointList[810]; // 10 * 9 * 9
int numPoints[10];
#define DIGIT_OFFSET(num) (&pointList[81*(num)])

static inline uint16_t squareRoot(uint16_t x) {
  uint16_t a, b;

  b = x;
  a = x = 0x3f;
  x = b/x;
  a = x = (x+a)>>1;
  x = b/x;
  a = x = (x+a)>>1;
  x = b/x;
  x = (x+a)>>1;

  return x;
}

static inline int32_t myArccos(int16_t x) {
  if (x < -512) x = -512;
  if (x > 512) x = 512;

  return __ACOS[512+x];
}

static inline int32_t length(const GPoint3 *v) {
  return squareRoot(v->x*v->x + v->y*v->y + v->z*v->z);
}

static void angles(const GPoint3 *v, int32_t *ax, int32_t *ay, int32_t *az) {
  float s = length(v);
  *ax = myArccos((v->y << 9)/s) - TRIG_MAX_ANGLE_BY_4;
  *ay = myArccos((v->x << 9)/s) - TRIG_MAX_ANGLE_BY_4;
  *az = 0;

  //	APP_LOG(APP_LOG_LEVEL_DEBUG, "length(%d, %d, %d) = %d / angles : %d %d %d", v->x, v->y, v->z, (int)s, (int)*ax, (int)*ay, (int)*az);
}

static inline void transformPoint(const GPoint3 *P, GPoint3 *T) {
  GPoint3 U;

  U.x = P->x << SHIFT;
  U.y = P->y << SHIFT;
  U.z = P->z << SHIFT;

  T->x = U.x;
  T->y = (U.y*cosa + U.z*sina) / TRIG_MAX_RATIO;
  T->z = (U.z*cosa - U.y*sina) / TRIG_MAX_RATIO;

  U.x = (T->x*cosb - T->z*sinb) / TRIG_MAX_RATIO;
  U.y = T->y;
  U.z = (T->x*sinb + T->z*cosb) / TRIG_MAX_RATIO;

  T->x = ((U.x*cosc + U.y*sinc) / TRIG_MAX_RATIO) >> SHIFT;
  T->y = ((U.y*cosc - U.x*sinc) / TRIG_MAX_RATIO) >> SHIFT;
  T->z = U.z >> SHIFT;
}

static inline void projectPoint(const GPoint3 *T, GPoint *S) {
  S->x = (eye.z * T->x) / (eye.z + T->z) + 72;
  S->y = (eye.z * T->y) / (eye.z + T->z) + 84;
}

//static int minZ = 100000, maxZ = -100000;

static void drawPoint(GContext *ctx, const GPoint3 *P) {
  GPoint S;
  GPoint3 T;

  transformPoint(P, &T);
  projectPoint(&T, &S);

  graphics_fill_rect(ctx, GRect(S.x-1, S.y-1, 4, 4), 1, GCornersAll);
  //if (T.z < minZ) minZ = T.z;
  //if (T.z > maxZ) maxZ = T.z;
}

static void updateLayer(Layer *layer, GContext *ctx) {
  int i, n, curd, t;
  GPoint3 P, *U;
  static int a = SIZE+2*OFFSET;
  static int twoa = 2*SIZE+4*OFFSET;
  static int step = 0;

  if ( (hour >= nightStopHour) && (hour < nightStartHour) ) {
    // DAY
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
  } else {
    //NIGHT
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_rect(ctx, fullScreenRect, 0, GCornerNone);
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_fill_color(ctx, GColorBlack);
  }

  for (n=0; n<6; n++) {
    curd = d[n];
    for (i=0, U=DIGIT_OFFSET(curd); i<numPoints[curd]; i++, U++) {
      P.x = U->x + digitOffsetX[n];
      P.y = U->y + digitOffsetY[n];
      t = ((((P.x>>1) + (P.y>>1) + step)%twoa) * TRIG_MAX_ANGLE) / twoa;
      P.z = U->z + (cos_lookup(t)*6)/TRIG_MAX_RATIO;
      drawPoint(ctx, &P);
    }
  }

  step = (step+4)%twoa;
  /*
   if (!step) {
   APP_LOG(APP_LOG_LEVEL_DEBUG, "minZ=%d, maxZ=%d", minZ, maxZ);
   }
   */
}

static void timerCallback(void *data) {
  static AccelData ad;
  static GPoint3 g;

  accel_service_peek(&ad);
  g.x = (3*g.x + (ad.x>>5)) >> 2;
  g.y = (3*g.y + (ad.y>>5)) >> 2;
  g.z = (3*g.z + (ad.z>>5)) >> 2;

  angles(&g, &a, &b, &c);

  cosa = cos_lookup(a);
  cosb = cos_lookup(b);
  cosc = cos_lookup(c);
  sina = sin_lookup(a);
  sinb = sin_lookup(b);
  sinc = sin_lookup(c);

  layer_mark_dirty(layer);

  timer = app_timer_register(TIMERDELAY, timerCallback, NULL);
}

static void handleTick(struct tm *t, TimeUnits units_changed) {
  int h = t->tm_hour;
  hour = h;

  if (!clock_is_24h_style()) {
    h %= 12;
    if (h==0) h = 12;
  }

  d[0] = h/10;
  d[1] = h%10;
  d[2] = t->tm_min/10;
  d[3] = t->tm_min%10;
  d[4] = t->tm_sec/10;
  d[5] = t->tm_sec%10;
}


static void handleAccel(AccelData *data, uint32_t num_samples) {
  // return;
}

static void logVariables(const char *msg) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "MSG: %s\n\tnightStartHour=%d\n\tnightStopHour=%d\n", msg, nightStartHour, nightStopHour);
}

static bool checkAndSaveInt(int *var, int val, int key) {
  status_t ret;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "CheckAndSaveInt : var=%d, val=%d, key=%d", *var, val, key);

  if (*var != val) {
    *var = val;
    ret = persist_write_int(key, val);
    if (ret < 0) APP_LOG(APP_LOG_LEVEL_DEBUG, "ERROR: persist_write_int returned %d", (int)ret);
    return true;
  } else {
    return false;
  }
}

void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "in_dropped_handler, reason: %d", reason);
}

void in_received_handler(DictionaryIterator *received, void *context) {
  Tuple *nightStart = dict_find(received, CONFIG_KEY_NIGHTSTART);
  Tuple *nightStop = dict_find(received, CONFIG_KEY_NIGHTSTOP);

  if (nightStart && nightStop) {
    checkAndSaveInt(&nightStartHour, nightStart->value->int32, CONFIG_KEY_NIGHTSTART);
    checkAndSaveInt(&nightStopHour, nightStop->value->int32, CONFIG_KEY_NIGHTSTOP);

    logVariables("ReceiveHandler");
  }
}

static void app_message_init(void) {
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_open(64, 64);
}

void readConfig() {
  if (persist_exists(CONFIG_KEY_NIGHTSTART)) {
    nightStartHour = persist_read_int(CONFIG_KEY_NIGHTSTART);
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "persist_exists(CONFIG_KEY_NIGHTSTART) returned false");
    nightStartHour = 19;
  }

  if (persist_exists(CONFIG_KEY_NIGHTSTOP)) {
    nightStopHour = persist_read_int(CONFIG_KEY_NIGHTSTOP);
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "persist_exists(CONFIG_KEY_NIGHTSTOP) returned false");
    nightStopHour = 7;
  }

  logVariables("readConfig");
}

static void initPointList() {
  int i, j, n, px, py;
  GPoint3 *P;

  for (n=0; n<10; n++) {
    P = DIGIT_OFFSET(n);
    numPoints[n] = 0;

    for (j=0; j<9; j++) {
      py = (j-4)*(SIZE>>3);

      for (i=0; i<9; i++) {
        px = (i-4)*(SIZE>>3);

        if (DIGIT(n, i, j)) {
          P->x = px;
          P->y = py;
          P->z = ZPOS;
          P++;
          numPoints[n]++;
        }
      }
    }
  }
}

static void initDigitOffsets() {
  int n;

  for (n=0; n<6; n++) {
    digitOffsetX[n] = (2*(n%2)-1)*((SIZE>>1)+OFFSET);
    digitOffsetY[n] = ((n/2)-1)*(SIZE+2*OFFSET);
  }
}

static void init(void) {
  time_t now;

  initDigitOffsets();
  initPointList();

  readConfig();
  app_message_init();

  time(&now);
  handleTick(localtime(&now), 0);

  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_stack_push(window, true);
  rootLayer = window_get_root_layer(window);

  layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(layer, updateLayer);
  layer_add_child(rootLayer, layer);

  tick_timer_service_subscribe(SECOND_UNIT, handleTick);
  
  accel_data_service_subscribe(0, handleAccel);
  timerCallback(NULL);
}

static void deinit(void) {
  app_timer_cancel(timer);
  accel_data_service_unsubscribe();
  tick_timer_service_unsubscribe();
  layer_destroy(layer);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
