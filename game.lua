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

    draw_image(x, y, qinghe)
end

function destroy()
    deinit_audio(audio)
    deinit_image(qinghe)
end

init_engine(1200, 750)