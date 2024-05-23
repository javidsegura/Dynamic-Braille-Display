import gradio as gr
import funcs

"""

HOW TO SETUP

1. Connect cable to arduino
1.5. (Possibly) open the arduino IDE
2. Run this code 



"""


demo = gr.Blocks()

with demo:
    gr.Markdown("## Speech or Text Input to Arduino")
    gr.Markdown("<h6> <i> Dynamic Braille Display </i></h6>")
    with gr.Row(equal_height=False):
        with gr.Column():
            text_input = gr.Textbox(lines=1, placeholder="Type here...", label="Text Input")
        with gr.Column():
            audio_input = gr.Audio(sources=["microphone"], label="Audio Input")
    confirm_button = gr.Button("Send message")
    output_text = gr.Textbox(label="Arduino's receiver")

    confirm_button.click(fn=funcs.process_input, inputs=[audio_input, text_input], outputs=output_text)

# Setting share to True in order to have access through the web
demo.launch(share=True)

