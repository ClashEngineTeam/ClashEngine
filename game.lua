title = "123"

x = 0
y = 0

buttonPosX = 500
buttonPosY = 400
buttonSizeX = 200
buttonSizeY = 100

r = 255
g = 0
b = 0

draw_red = true

function mouse_click(btn)
    if (draw_red == true) then
        draw_red = false
        r = 0
        g = 255
    else
        draw_red = true
        r = 255
        g = 0
    end
end

function draw_my_button(btn)
    buttonPos = get_ui_pos(btn)
    buttonSize = get_ui_size(btn)

    draw_image_scaling(buttonPos:x(), buttonPos:y(), button_image, buttonSize:x(), buttonSize:y())
    draw_frame(buttonPos:x(), buttonPos:y(), buttonSize:x(), buttonSize:y(), r, g, b)
    --draw_image_scaling(buttonPosX, buttonPosY, button_image, buttonSizeX, buttonSizeY)
    --draw_frame(buttonPosX,buttonPosY,buttonSizeX,buttonSizeY, r,g,b)
end

function start()

    --random_value = random_range(0,1)

    set_window_title(title)

    audio = init_audio("res/audios/sgs_background_music_default.mp3")
    qinghe = init_image("res/images/characters/QingHe2.png")
    hello_world = init_font("res/fonts/msyh.ttc", 72)

    --console = init_console()
    
    --load button image:
    button_image = init_image("res/images/buttons/start.png")
    
    button = init_button(buttonPosX, buttonPosY, buttonSizeX, buttonSizeY)
    bind_mouse_click(button, "mouse_click")
    bind_draw(button, "draw_my_button")
    
    --disable_ui(button)
    newButton = init_button(buttonPosX + 10, buttonPosY + 10, 100, 100)
    set_ui_parent(newButton, button)
    
    pos = get_ui_local_pos(newButton)
    --writeline_console(console, tostring(pos:x()))
    --writeline_console(console, tostring(pos:y()))

    --Vector2:
    --position = Vector2.new(10, 10)
    --writeline_console(console, tostring(position:x()))

    --a = ui_active(newButton)
    --writeline_console(console, tostring(a))

    --p = get_ui_parent(newButton)
    --writeline_console(console, tostring(p))
    --writeline_console(console, get_ui_local_pos_x(newButton))

    --disable_ui(button)
    --play_audio(audio, true, false)

    --font_sprite = render_font_to_sprite(hello_world, title, 255, 255, 255)

    test_font = render_font_to_sprite(hello_world, title, 0,0,0)
    font_sprite = render_font_to_sprite_ex(hello_world, title, 0, 0, 0, 50)

    inputField = init_input_field(400, 320, 400, 40, hello_world)

end

function update()
    --clear()
    clear_color(155,55,155)

    if GetKey(Key_A) then
        x = x - 1
    end
    if GetKey(Key_D) then
        x = x + 1
    end
    if GetKey(Key_W) then
        y = y - 1
    end
    if GetKey(Key_S) then
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
    --draw_image(x, y, qinghe)
    --draw_image_scaling(x, y, qinghe, 2000, 1300)
    
    if GetKeyDown(Key_F1) then
        console = init_console()
        write_console(console, "你好乐基")
    end

    if GetKeyDown(Key_F2) then
        deinit_console(console)
    end

    draw_image_scalingf(x, y, qinghe, 1, 1)

    --draw_image_alpha(x,y,qinghe,100)

    --draw_image_scaling(x,y,qinghe,1200,750)

    width = get_font_offset_y(hello_world, "高玩侠")
    dt = get_delta_time()
    
    --draw_rect(0,0,100,200, 255,0,0)
    --draw_frame(100,100,200,200, 255,255,0)

    --fw = get_font_width(hello_world, title)
    --draw_font_ex(hello_world, title, 600 - double2int(fw / 2), 200, 0, 0, 0, 50)

    mx = get_mouse_x()
    my = get_mouse_y()
    
    draw_ui(button)
    draw_ui(newButton)
    draw_ui(inputField)
    inputData = get_input_field_data(inputField)
    draw_font(hello_world, inputData, 300,300,255,255,255)

    --draw_font(hello_world, title, 400, 100, 0,0,0)

    --draw_png_image(400,100,test_font)
    draw_png_image(500, 200, font_sprite)

    --draw_font_ex(hello_world, title, 400, 100, 0,0,0,30)
    --draw_png_image_scalingf(400, 100, font_sprite, 2, 2)

    --draw_image(100, 100, font_sprite)

    --writeline_console(console, mx)
    --writeline_console(console, dt)

    -- sw = get_screen_width()
    -- sh = get_screen_height()
    -- writeline_console(console, tostring(sw))
    -- writeline_console(console, tostring(sh))

    --write_console(console, width)
    --draw_line(0,0, 1000,700, 0,0,0)
    --draw_line_bold(0,0, 1000,700, 0,0,0,3)
end

function destroy()
    deinit_audio(audio)
    deinit_image(qinghe)
    --deinit_console(console)
end

init_engine(1200, 750)