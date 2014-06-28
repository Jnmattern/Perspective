#include <pebble.h>

#define M_PI		3.14159265358979323846

#define SIZE 40
#define OFFSET 5
#define ZPOS -20
#define EYEZ 200
#define SHIFT 8

enum {
	CONFIG_KEY_NIGHTSTART = 3333,
	CONFIG_KEY_NIGHTSTOP = 3334
};


static Window *window;
static Layer *rootLayer, *layer;
static AppTimer *timer;
static int32_t a=0, b=0, c=0;
static int32_t cosa, sina, cosb, sinb, cosc, sinc;
static int hour;
static int d[6];
static GRect fullScreenRect = { { 0, 0 }, { 144, 168 } };
static int nightStartHour = 19, nightStopHour = 7;

static const uint16_t __ACOS[1001] = {
	32768, 32108, 31834, 31624, 31447, 31291, 31150, 31020, 30899, 30785, 30678, 30576, 30478, 30384, 30293, 30206, 30122, 30040, 29960, 29883,
	29807, 29734, 29662, 29592, 29523, 29455, 29389, 29324, 29260, 29198, 29136, 29075, 29016, 28957, 28899, 28842, 28785, 28730, 28675, 28621,
	28567, 28514, 28462, 28410, 28359, 28308, 28258, 28209, 28160, 28111, 28063, 28015, 27968, 27922, 27875, 27829, 27784, 27738, 27694, 27649,
	27605, 27561, 27518, 27475, 27432, 27390, 27348, 27306, 27264, 27223, 27182, 27141, 27101, 27060, 27020, 26981, 26941, 26902, 26863, 26824,
	26786, 26747, 26709, 26671, 26633, 26596, 26559, 26522, 26485, 26448, 26411, 26375, 26339, 26303, 26267, 26231, 26196, 26161, 26125, 26090,
	26056, 26021, 25986, 25952, 25918, 25884, 25850, 25816, 25782, 25749, 25715, 25682, 25649, 25616, 25583, 25550, 25518, 25485, 25453, 25420,
	25388, 25356, 25324, 25292, 25261, 25229, 25198, 25166, 25135, 25104, 25073, 25042, 25011, 24980, 24949, 24919, 24888, 24858, 24828, 24798,
	24767, 24737, 24708, 24678, 24648, 24618, 24589, 24559, 24530, 24500, 24471, 24442, 24413, 24384, 24355, 24326, 24297, 24269, 24240, 24211,
	24183, 24155, 24126, 24098, 24070, 24042, 24014, 23986, 23958, 23930, 23902, 23874, 23847, 23819, 23791, 23764, 23736, 23709, 23682, 23655,
	23627, 23600, 23573, 23546, 23519, 23492, 23466, 23439, 23412, 23385, 23359, 23332, 23306, 23279, 23253, 23226, 23200, 23174, 23148, 23122,
	23095, 23069, 23043, 23017, 22992, 22966, 22940, 22914, 22888, 22863, 22837, 22811, 22786, 22760, 22735, 22710, 22684, 22659, 22634, 22608,
	22583, 22558, 22533, 22508, 22483, 22458, 22433, 22408, 22383, 22358, 22333, 22309, 22284, 22259, 22235, 22210, 22185, 22161, 22136, 22112,
	22087, 22063, 22039, 22014, 21990, 21966, 21941, 21917, 21893, 21869, 21845, 21821, 21797, 21773, 21749, 21725, 21701, 21677, 21653, 21629,
	21606, 21582, 21558, 21534, 21511, 21487, 21463, 21440, 21416, 21393, 21369, 21346, 21322, 21299, 21275, 21252, 21229, 21205, 21182, 21159,
	21136, 21112, 21089, 21066, 21043, 21020, 20997, 20974, 20951, 20927, 20904, 20882, 20859, 20836, 20813, 20790, 20767, 20744, 20721, 20699,
	20676, 20653, 20630, 20608, 20585, 20562, 20540, 20517, 20494, 20472, 20449, 20427, 20404, 20382, 20359, 20337, 20314, 20292, 20269, 20247,
	20225, 20202, 20180, 20158, 20135, 20113, 20091, 20069, 20046, 20024, 20002, 19980, 19958, 19936, 19913, 19891, 19869, 19847, 19825, 19803,
	19781, 19759, 19737, 19715, 19693, 19671, 19649, 19627, 19605, 19583, 19562, 19540, 19518, 19496, 19474, 19452, 19431, 19409, 19387, 19365,
	19344, 19322, 19300, 19278, 19257, 19235, 19213, 19192, 19170, 19149, 19127, 19105, 19084, 19062, 19041, 19019, 18998, 18976, 18954, 18933,
	18911, 18890, 18869, 18847, 18826, 18804, 18783, 18761, 18740, 18718, 18697, 18676, 18654, 18633, 18612, 18590, 18569, 18548, 18526, 18505,
	18484, 18462, 18441, 18420, 18399, 18377, 18356, 18335, 18314, 18292, 18271, 18250, 18229, 18208, 18186, 18165, 18144, 18123, 18102, 18081,
	18060, 18038, 18017, 17996, 17975, 17954, 17933, 17912, 17891, 17870, 17849, 17828, 17806, 17785, 17764, 17743, 17722, 17701, 17680, 17659,
	17638, 17617, 17596, 17575, 17554, 17533, 17512, 17491, 17470, 17449, 17428, 17407, 17386, 17365, 17344, 17324, 17303, 17282, 17261, 17240,
	17219, 17198, 17177, 17156, 17135, 17114, 17093, 17072, 17052, 17031, 17010, 16989, 16968, 16947, 16926, 16905, 16884, 16863, 16843, 16822,
	16801, 16780, 16759, 16738, 16717, 16696, 16676, 16655, 16634, 16613, 16592, 16571, 16550, 16530, 16509, 16488, 16467, 16446, 16425, 16404,
	16384, 16363, 16342, 16321, 16300, 16279, 16258, 16237, 16217, 16196, 16175, 16154, 16133, 16112, 16091, 16071, 16050, 16029, 16008, 15987,
	15966, 15945, 15924, 15904, 15883, 15862, 15841, 15820, 15799, 15778, 15757, 15736, 15715, 15695, 15674, 15653, 15632, 15611, 15590, 15569,
	15548, 15527, 15506, 15485, 15464, 15443, 15423, 15402, 15381, 15360, 15339, 15318, 15297, 15276, 15255, 15234, 15213, 15192, 15171, 15150,
	15129, 15108, 15087, 15066, 15045, 15024, 15003, 14982, 14961, 14939, 14918, 14897, 14876, 14855, 14834, 14813, 14792, 14771, 14750, 14729,
	14707, 14686, 14665, 14644, 14623, 14602, 14581, 14559, 14538, 14517, 14496, 14475, 14453, 14432, 14411, 14390, 14368, 14347, 14326, 14305,
	14283, 14262, 14241, 14219, 14198, 14177, 14155, 14134, 14113, 14091, 14070, 14049, 14027, 14006, 13984, 13963, 13941, 13920, 13898, 13877,
	13856, 13834, 13813, 13791, 13769, 13748, 13726, 13705, 13683, 13662, 13640, 13618, 13597, 13575, 13554, 13532, 13510, 13489, 13467, 13445,
	13423, 13402, 13380, 13358, 13336, 13315, 13293, 13271, 13249, 13227, 13205, 13184, 13162, 13140, 13118, 13096, 13074, 13052, 13030, 13008,
	12986, 12964, 12942, 12920, 12898, 12876, 12854, 12831, 12809, 12787, 12765, 12743, 12721, 12698, 12676, 12654, 12632, 12609, 12587, 12565,
	12542, 12520, 12498, 12475, 12453, 12430, 12408, 12385, 12363, 12340, 12318, 12295, 12273, 12250, 12227, 12205, 12182, 12159, 12137, 12114,
	12091, 12068, 12046, 12023, 12000, 11977, 11954, 11931, 11908, 11885, 11863, 11840, 11816, 11793, 11770, 11747, 11724, 11701, 11678, 11655,
	11631, 11608, 11585, 11562, 11538, 11515, 11492, 11468, 11445, 11421, 11398, 11374, 11351, 11327, 11304, 11280, 11256, 11233, 11209, 11185,
	11161, 11138, 11114, 11090, 11066, 11042, 11018, 10994, 10970, 10946, 10922, 10898, 10874, 10850, 10826, 10801, 10777, 10753, 10728, 10704,
	10680, 10655, 10631, 10606, 10582, 10557, 10532, 10508, 10483, 10458, 10434, 10409, 10384, 10359, 10334, 10309, 10284, 10259, 10234, 10209,
	10184, 10159, 10133, 10108, 10083, 10057, 10032, 10007, 9981, 9956, 9930, 9904, 9879, 9853, 9827, 9801, 9775, 9750, 9724, 9698,
	9672, 9645, 9619, 9593, 9567, 9541, 9514, 9488, 9461, 9435, 9408, 9382, 9355, 9328, 9301, 9275, 9248, 9221, 9194, 9167,
	9140, 9112, 9085, 9058, 9031, 9003, 8976, 8948, 8920, 8893, 8865, 8837, 8809, 8781, 8753, 8725, 8697, 8669, 8641, 8612,
	8584, 8556, 8527, 8498, 8470, 8441, 8412, 8383, 8354, 8325, 8296, 8267, 8237, 8208, 8178, 8149, 8119, 8089, 8059, 8030,
	8000, 7969, 7939, 7909, 7879, 7848, 7818, 7787, 7756, 7725, 7694, 7663, 7632, 7601, 7569, 7538, 7506, 7475, 7443, 7411,
	7379, 7347, 7314, 7282, 7249, 7217, 7184, 7151, 7118, 7085, 7052, 7018, 6985, 6951, 6917, 6883, 6849, 6815, 6781, 6746,
	6711, 6677, 6642, 6606, 6571, 6536, 6500, 6464, 6428, 6392, 6356, 6319, 6282, 6245, 6208, 6171, 6134, 6096, 6058, 6020,
	5981, 5943, 5904, 5865, 5826, 5786, 5747, 5707, 5666, 5626, 5585, 5544, 5503, 5461, 5419, 5377, 5335, 5292, 5249, 5206,
	5162, 5118, 5073, 5029, 4983, 4938, 4892, 4845, 4799, 4752, 4704, 4656, 4607, 4558, 4509, 4459, 4408, 4357, 4305, 4253,
	4200, 4146, 4092, 4037, 3982, 3925, 3868, 3810, 3751, 3692, 3631, 3569, 3507, 3443, 3378, 3312, 3244, 3175, 3105, 3033,
	2960, 2884, 2807, 2727, 2645, 2561, 2474, 2383, 2289, 2191, 2089, 1982, 1868, 1747, 1617, 1476, 1320, 1143, 933, 659, 0
};

typedef struct {
	int x, y, z;
} GPoint3;

#define DIGIT(num, x, y) digit[(81*(num))+((y)*9)+(x)]

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

GPoint3 eye = { 0, 0, EYEZ };

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
	if (x < -500.0) x = -500.0;
	if (x > 500.0) x = 500.0;

	return __ACOS[500+x];
}

static inline int32_t length(const GPoint3 *v) {
	return squareRoot(v->x*v->x + v->y*v->y + v->z*v->z);
}

static void angles(const GPoint3 *v, int32_t *ax, int32_t *ay, int32_t *az) {
	float s = length(v);
	*ax = myArccos(500*v->y/s) - TRIG_MAX_ANGLE/4;
	*ay = myArccos(500*v->x/s) - TRIG_MAX_ANGLE/4;
	*az = 0;

	//	APP_LOG(APP_LOG_LEVEL_DEBUG, "length(%d, %d, %d) = %d / angles : %d %d %d", v->x, v->y, v->z, (int)s, (int)*ax, (int)*ay, (int)*az);
}

static inline void projectPoint(const GPoint3 *P, GPoint *S) {
	GPoint3 T, U;
	
	U.x = P->x << SHIFT;
	U.y = P->y << SHIFT;
	U.z = P->z << SHIFT;

	T.x = U.x;
	T.y = (U.y*cosa + U.z*sina) / TRIG_MAX_RATIO;
	T.z = (U.z*cosa - U.y*sina) / TRIG_MAX_RATIO;
	
	U.x = (T.x*cosb - T.z*sinb) / TRIG_MAX_RATIO;
	U.y = T.y;
	U.z = (T.x*sinb + T.z*cosb) / TRIG_MAX_RATIO;
	
	T.x = ((U.x*cosc + U.y*sinc) / TRIG_MAX_RATIO) >> SHIFT;
	T.y = ((U.y*cosc - U.x*sinc) / TRIG_MAX_RATIO) >> SHIFT;
	T.z = U.z >> SHIFT;
	
	S->x = (eye.z * T.x) / (eye.z + T.z) + 72;
	S->y = (eye.z * T.y) / (eye.z + T.z) + 84;
}

static void drawPoint(GContext *ctx, const GPoint3 *P) {
	GPoint S;
	
	projectPoint(P, &S);
	graphics_fill_rect(ctx, GRect(S.x-1, S.y-1, 4, 4), 1, GCornersAll);
	//graphics_fill_circle(ctx, S, 1);
}

static void updateLayer(Layer *layer, GContext *ctx) {
	int i, j, n;
	GPoint3 U;

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

	for (j=0; j<9; j++) {
		for (i=0; i<9; i++) {
			for (n=0; n<6; n++) {
				if (DIGIT(d[n], i, j)) {
					U.x = (i-4)*(SIZE>>3) + (2*(n%2)-1)*((SIZE>>1)+OFFSET);
					U.y = (j-4)*(SIZE>>3) + ((n/2)-1)*(SIZE+2*OFFSET);
					U.z = ZPOS;
					drawPoint(ctx, &U);
				}
			}
		}
	}
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
	
	timer = app_timer_register(100, timerCallback, NULL);
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

static void init(void) {
	time_t now;

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
