function start()
    audio = init_audio("res/audios/sgs_background_music_default.mp3")
    qinghe = init_image("res/images/characters/QingHe2.png")
    play_audio(audio, true, false)
end

function update()
    draw_image(0, 0, qinghe)
end

function destroy()
    deinit_audio(audio)
    deinit_image(qinghe)
end

init_engine(1200, 750)