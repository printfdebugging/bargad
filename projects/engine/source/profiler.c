#include "engine/profiler.h"
#include "engine/logger.h"

static struct profiler_slot *profiler_slots = NULL;

static double profiler_start_ms = 0;
static long profiler_frame_count = 0;

#define NL "\n"

static const char profiler_report_header[] = NL
	"┌─ PROFILER ───────────────────────────────┬────────────────────────────────────────────────┐" NL
	"│ avg frame time (ms)                      │ %-46.3f │" NL
	"│ avg frame rate (fps)                     │ %-46.1f │" NL
	"│ frames                                   │ %-46ld │" NL
	"├──────────────────────────────────────────┼────────────┬─────────┬────────────┬────────────┤" NL
	"│ name                                     │   ms/frame │       %% │     max ms │      calls │" NL
	"├──────────────────────────────────────────┼────────────┼─────────┼────────────┼────────────┤" NL;

static const char profiler_report_row[] =
	"│ %-40.40s │ %10.3f │ %6.1f%% │ %10.3f │ %10ld │" NL;

static const char profiler_report_footer[] =
	"└──────────────────────────────────────────┴────────────┴─────────┴────────────┴────────────┘" NL NL;

#undef NL

void
profiler_add(struct profiler_slot *slot, double ms_elapsed)
{
	if(slot->call_count == 0) {
		slot->next = profiler_slots;
		profiler_slots = slot;
	}

	slot->call_count++;
	slot->ms.total += ms_elapsed;

	if(ms_elapsed > slot->ms.max)
		slot->ms.max = ms_elapsed;
	if(slot->ms.limit > 0.0 && ms_elapsed > slot->ms.limit)
		LOG_WARNING("%s took %.3f ms (limit %.3f ms) at %s:%d\n", slot->name, ms_elapsed, slot->ms.limit, slot->file, slot->line);
}

void
profiler_frame(void)
{
	if(profiler_frame_count == 0) {
		profiler_start_ms = profiler_now_ms();
	}
	profiler_frame_count++;
}

void
profiler_report(void)
{
	if(profiler_frame_count == 0) {
		LOG_WARNING("profiler_frame_count is 0, please call profiler_frame each frame\n");
		return;
	}

	double frame_ms = (profiler_now_ms() - profiler_start_ms) / (double)profiler_frame_count;
	printf(profiler_report_header, frame_ms, 1000.0 / frame_ms, profiler_frame_count);

	for(struct profiler_slot *slot = profiler_slots; slot; slot = slot->next) {
		double ms_per_frame = slot->ms.total / (double)profiler_frame_count;
		printf(profiler_report_row, slot->name, ms_per_frame, ms_per_frame / frame_ms * 100.0, slot->ms.max, slot->call_count);
	}

	fprintf(stdout, profiler_report_footer);
}
