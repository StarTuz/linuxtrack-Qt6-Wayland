-- gaze_phase0_test.lua
--
-- Live test harness for the X-Plane gaze DataRef prototype (Phase 0 of
-- doc/GAZE_TRACKING_ROADMAP.md). Drop into FlyWithLua's Scripts directory:
--
--   ~/X-Plane 12/Resources/plugins/FlyWithLua/Scripts/gaze_phase0_test.lua
--
-- With LTR_GAZE_MOCK enabled in src/xlinuxtrack9.c, this script asserts the
-- mock gaze DataRefs are alive, bounded, and changing. It logs to
-- Log.txt every two seconds with PASS / FAIL lines so you can confirm
-- without alt-tabbing to DataRefEditor.
--
-- When Phase 3 wires in real gaze (linuxtrack_get_gaze) and you set
-- LTR_GAZE_MOCK to 0, the bounds checks here become bounds checks on real
-- gaze instead — adjust YAW_LIMIT_DEG / PITCH_LIMIT_DEG as appropriate.

dataref("ltr_gaze_yaw",   "linuxtrack/gaze_yaw",   "readable")
dataref("ltr_gaze_pitch", "linuxtrack/gaze_pitch", "readable")

local YAW_LIMIT_DEG   = 35.0  -- mock peaks at 30, give 5° slack
local PITCH_LIMIT_DEG = 20.0  -- mock peaks at 15, give 5° slack
local CHECK_INTERVAL_SEC = 2.0

local last_check = 0.0
local last_yaw   = nil
local last_pitch = nil
local stuck_ticks = 0

function gaze_phase0_check()
    local now = os.clock()
    if (now - last_check) < CHECK_INTERVAL_SEC then return end
    last_check = now

    local yaw   = ltr_gaze_yaw
    local pitch = ltr_gaze_pitch

    local bounded =
        math.abs(yaw)   <= YAW_LIMIT_DEG and
        math.abs(pitch) <= PITCH_LIMIT_DEG

    local changing = false
    if last_yaw ~= nil then
        changing = (yaw ~= last_yaw) or (pitch ~= last_pitch)
        if not changing then
            stuck_ticks = stuck_ticks + 1
        else
            stuck_ticks = 0
        end
    end
    last_yaw   = yaw
    last_pitch = pitch

    local status = "PASS"
    if not bounded                then status = "FAIL bounds"  end
    if last_yaw and stuck_ticks > 1 then status = "FAIL stuck" end

    logMsg(string.format(
        "gaze_phase0: %s yaw=%+.2f pitch=%+.2f (limits %.0f/%.0f, stuck=%d)",
        status, yaw, pitch, YAW_LIMIT_DEG, PITCH_LIMIT_DEG, stuck_ticks))
end

do_every_frame("gaze_phase0_check()")

logMsg("gaze_phase0_test.lua loaded — watching linuxtrack/gaze_{yaw,pitch}")
