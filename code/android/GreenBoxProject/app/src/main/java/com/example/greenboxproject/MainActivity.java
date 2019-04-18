package com.example.greenboxproject;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import org.json.JSONException;
import org.json.JSONObject;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLConnection;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        class GetData implements Runnable {
            StringBuilder output = new StringBuilder();
            String  temperature;
            String  pressure;
            String  humidity;
            String  dustDensity;
            String  lpg;
            String  co;
            String  smoke;
            String voltage;


            public void run() {
                try {

                    while(true){
                        URL oracle = new URL("http://77.76.185.128:4343/"); // URL to Parse
                        URLConnection yc = oracle.openConnection();
                        BufferedReader in = new BufferedReader(new InputStreamReader(yc.getInputStream()));
                        String inputLine;
                        while ((inputLine = in.readLine()) != null) {
                            output.append(inputLine);
                        }
                        in.close();

                        //Parse the JSON!
                             JSONObject mainObject = new JSONObject(output.toString());
                             temperature = mainObject.getString("temperature");
                             pressure = mainObject.getString("pressure");
                             humidity = mainObject.getString("humidity");
                             dustDensity = mainObject.getString("dustDensity");
                             lpg = mainObject.getString("lpg");
                             co = mainObject.getString("co");
                             smoke = mainObject.getString("smoke");
                             voltage = mainObject.getString("voltage");

                         output = new StringBuilder();

                    }
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }

            public String getTemperature() {
                if (temperature == null)
                    return "";
                else return temperature;
            }

            public String getHumidity() {
                if (humidity == null)
                    return "";
                else return humidity;
            }

            public String getPressure() {
                if (pressure == null)
                    return "";
                else return pressure;
            }

            public String getDustDensity() {
                if (dustDensity == null)
                    return "";
                else return dustDensity;
            }

            public String getLpg() {
                if (lpg == null)
                    return "";
                else return lpg;
            }

            public String getCo() {
                if (co == null)
                    return "";
                else return co;
            }

            public String getSmoke() {
                if (smoke == null)
                    return "";
                else return smoke;
            }

            public String getVoltage() {
                if (voltage == null)
                    return "";
                else return voltage;
            }
        }

        final GetData test = new GetData();
        Thread thread = new Thread(test);
        thread.start();

        final Handler ha = new Handler();
        ha.postDelayed(new Runnable() {
            @Override
            public void run() {
                TextView temp = findViewById(R.id.tempText);
                TextView press = findViewById(R.id.pressText);
                TextView dust = findViewById(R.id.dustText);
                TextView coGas = findViewById(R.id.gasCOText);
                TextView lpgGas = findViewById(R.id.gasLPGText);
                TextView smokeGas = findViewById(R.id.gasSmokeText);
                TextView humid = findViewById(R.id.humidityText);
                TextView battery = findViewById(R.id.batteryText);
                if(test.getTemperature() != "")
                {
                    temp.setText(test.getTemperature());
                }
                if(test.getHumidity() != "")
                {
                    humid.setText(test.getHumidity());
                }
                if(test.getPressure() != "")
                {
                    press.setText(test.getPressure());
                }
                if(test.getDustDensity() != "")
                {
                    dust.setText(test.getDustDensity());
                    if(Double.parseDouble(test.getDustDensity())>= 0.40){
                        ImageView dustImg = findViewById(R.id.dustImg);
                        dustImg.setColorFilter(Color.parseColor("#e83410"));
                    }
                    else {
                        ImageView dustImg = findViewById(R.id.dustImg);
                        dustImg.setColorFilter(Color.parseColor("#ff99cc00"));
                    }
                }
                if(test.getCo() != "")
                {
                    coGas.setText(test.getCo());
                    if(Double.parseDouble(test.getCo()) >= 10.00){
                        ImageView dustImg = findViewById(R.id.gasCOImg);
                        dustImg.setBackgroundColor(Color.parseColor("#e83410"));
                    }
                    else {
                        ImageView dustImg = findViewById(R.id.gasCOImg);
                        dustImg.setColorFilter(Color.parseColor("#ff99cc00"));
                    }
                }
                if(test.getLpg() != "")
                {
                    lpgGas.setText(test.getLpg());
                    if(Double.parseDouble(test.getLpg())>= 10){
                        ImageView dustImg = findViewById(R.id.gasLPGImg);
                        dustImg.setBackgroundColor(Color.parseColor("#e83410"));
                    }
                    else {
                        ImageView dustImg = findViewById(R.id.gasLPGImg);
                        dustImg.setColorFilter(Color.parseColor("#ff99cc00"));
                    }
                }
                if(test.getSmoke() != "")
                {
                    smokeGas.setText(test.getSmoke());
                    if(Double.parseDouble(test.getSmoke())>= 10){
                        ImageView dustImg = findViewById(R.id.gasSmokeImg);
                        dustImg.setBackgroundColor(Color.parseColor("#e83410"));
                    }
                    else {
                        ImageView dustImg = findViewById(R.id.gasSmokeImg);
                        dustImg.setColorFilter(Color.parseColor("#ff99cc00"));
                    }
                }
                if(test.getVoltage() != "")
                {
                    battery.setText( String.format("%.2f", ((Double.parseDouble(test.getVoltage())-3.4)*100)) + "%");
                    if(Double.parseDouble(test.getVoltage()) - 1 >= 10){
                        ImageView dustImg = findViewById(R.id.gasSmokeImg);
                        dustImg.setBackgroundColor(Color.parseColor("#e83410"));
                    }
                    else {
                        ImageView dustImg = findViewById(R.id.gasSmokeImg);
                        dustImg.setColorFilter(Color.parseColor("#ff99cc00"));
                    }
                }

                ha.postDelayed(this, 500);
            }
        },500);
    }

}


