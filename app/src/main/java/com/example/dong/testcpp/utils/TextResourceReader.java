package com.example.dong.testcpp.utils;

import android.content.Context;
import android.content.res.Resources;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created by fyd225 on 2017/5/17.
 */

public class TextResourceReader {

    public static String readTextFileFromResource(Context context, int resourceId) {
        StringBuilder body = new StringBuilder();

        try {
            InputStream inputStream = context.getResources().openRawResource(resourceId);
            InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

            String nextLine;

            while ((nextLine = bufferedReader.readLine()) != null) {
                body.append(nextLine);
                body.append('\n');
            }
            body.deleteCharAt(body.length()-1);
        } catch (IOException e) {
            throw new RuntimeException("Could not open resource: " + resourceId, e);
        } catch (Resources.NotFoundException e) {
            throw  new RuntimeException("Resource not found: " + resourceId, e);
        }
        return body.toString();
    }

    public static String readTextFileFromAssert(Context context, String assertName) {
        StringBuilder body = new StringBuilder();

        try {
            InputStream inputStream = context.getAssets().open(assertName);
            InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

            String nextLine;

            while ((nextLine = bufferedReader.readLine()) != null) {
                body.append(nextLine);
                body.append('\n');
            }
            body.deleteCharAt(body.length()-1);
        } catch (IOException e) {
            throw new RuntimeException("Could not open resource: " + assertName, e);
        } catch (Resources.NotFoundException e) {
            throw  new RuntimeException("Resource not found: " + assertName, e);
        }
        return body.toString();
    }
}
