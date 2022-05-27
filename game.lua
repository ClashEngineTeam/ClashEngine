x = 0
y = 0

function start()
    audio = init_audio("res/audios/sgs_background_music_default.mp3")
    qinghe = init_image("res/images/characters/QingHe2.png")
    play_audio(audio, true, false)
end

function update()
    clear()

    if get_key(KEY_A) then
        x = x - 1
    end
    if get_key(KEY_D) then
        x = x + 1
    end
    if get_key(KEY_W) then
        y = y - 1
    end
    if get_key(KEY_S) then
        y = y + 1
    end

    xx = get_mouse_x()
    yy = get_mouse_y()
    ww = get_mouse_wheel()

    --鼠标上滑
    if ww > 0 then
        y = y + 10
    end

    --鼠标下滑
    if ww < 0 then
        y = y - 10
    end
    
    --draw_image(xx, yy, qinghe)
    draw_image(x, y, qinghe)
    --draw_line(0,0, 1000,700, 0,0,0)
    draw_line_bold(0,0, 1000,700, 0,0,0,3)
end

function destroy()
    deinit_audio(audio)
    deinit_image(qinghe)
end

init_engine(1200, 750)