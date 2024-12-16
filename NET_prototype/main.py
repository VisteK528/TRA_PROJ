import tensorflow as tf
from tensorflow.keras.utils import plot_model
import os

model = tf.keras.models.load_model("tra_prototype_model.h5")
model.summary()
plot_model(model, to_file='images/model.png', show_shapes=True)


converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.convert()

# # Save the quantized model
# with open('/home/vistek528/TRA_PROJ/NET_prototype/model_quantized.tflite', 'wb') as f:
#     f.write(tflite_model)
#
# model_Size = os.path.getsize("/home/vistek528/TRA_PROJ/NET_prototype/model_quantized.tflite")
# print(f"Model ma {model_Size} bajtów")