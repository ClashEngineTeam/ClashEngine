function start()
    font = init_font("res/fonts/msyh.ttc", 20)
end

function update()
    clear_color(255, 255, 255)
    draw_font(font, "Hello World", 20, 20, 0, 0, 255)
end

function destroy()
    deinit_font(font)
end

init_engine(800, 600)