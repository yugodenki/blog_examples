import soundfile as sf
import samplerate
import sys
import os


def main():

  if len(sys.argv) != 2:
    print("Usage: python converter.py <input file path>")
    sys.exit(1)
  
  input_file_path = sys.argv[1]
  input_directory, input_file_name = os.path.split(input_file_path)
  output_file_name = input_file_name.replace(".mp3", ".h")

  data_in, datasamplerate = sf.read(input_file_path)
  print(f"data sample rate: {datasamplerate}")
  # This means stereo so extract one channel 0
  if len(data_in.shape)>1:
      data_in = data_in[:,0]
  
  converter = 'sinc_best'  # or 'sinc_fastest', ...
  desired_sample_rate = 24000.0
  ratio = desired_sample_rate/datasamplerate
  data_out = samplerate.resample(data_in, ratio, converter)
  print(data_out)
  maxValue = max(data_out)
  minValue = min(data_out)
  print("length", len(data_out))
  print("max value", max(data_out))
  print("min value", min(data_out))
  vrange = (maxValue - minValue) 
  print("value range", vrange)

  m68code = f"/*    File {input_file_path} \r\n *    Sample rate {str(int(desired_sample_rate))} Hz\r\n */\r\n"
  m68code += "#define WAV_DATA_LENGTH "+str(len(data_out))+" \r\n\r\n"
  m68code += "const uint8_t WAV_DATA[] = {\r\n    "
  maxitemsperline = 16 
  itemsonline = maxitemsperline
  firstvalue = 0
  lastvalue = 0
  for v in data_out:
      # scale v to between 0 and 1
      isin = (v-minValue)/vrange   
      v =  int((isin * 255))
      vstr = f"0x{v:02X}"
      if (firstvalue==0):
          firstvalue= v
      lastvalue = v
      m68code+=vstr
      itemsonline-=1
      if (itemsonline>0):
          m68code+=','
      else:
          itemsonline = maxitemsperline
          m68code+=',\r\n    '
          
  # keep track of first and last values to avoid
  # blip when the loop restarts.. make the end value
  # the average of the first and last. 
  end_value = int( (firstvalue + lastvalue) / 2)
  m68code+=str(end_value)+'    \r\n};'

  with open(os.path.join(input_directory, output_file_name), "w") as f:
    f.write(m68code)


if __name__ == "__main__":
  main()